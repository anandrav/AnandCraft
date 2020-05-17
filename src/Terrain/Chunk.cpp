#include "Chunk.h"

#include "TerrainShader.h"
#include "TerrainTexture.h"
#include "../Game.h"
#include "../SaveError.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

Chunk::Chunk()
: active(false)
, loaded(false)
, dirty(false)
, can_render(false)
, pending_jobs(false)
{
}

void Chunk::set_active(ChunkCoords coords_) {
    active = true;
    coords = coords_;
    calculate_translation();
}

void Chunk::calculate_translation() {
    translation = glm::translate(glm::vec3{coords.x*CHUNK_WIDTH, coords.y*CHUNK_WIDTH, coords.z*CHUNK_WIDTH});
}

void Chunk::render_opaque(const Camera& camera) const
{
    shared_lock<shared_mutex> read_guard(mut);

    if (!can_render)
        return;
    glBindTexture(GL_TEXTURE_2D, TerrainTexture::get());
    glUseProgram(TerrainShader::ID());
    glm::mat4 clip_transform = camera.get_view_projection() * translation;
    glUniformMatrix4fv(glGetUniformLocation(TerrainShader::ID(), "transform"), 1, GL_FALSE, &clip_transform[0][0]);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    opaque_mesh.draw();
}

void Chunk::render_transparent(const Camera& camera) const 
{
    shared_lock<shared_mutex> read_guard(mut);

    if (!can_render)
        return;
    glBindTexture(GL_TEXTURE_2D, TerrainTexture::get());
    glUseProgram(TerrainShader::ID());
    glm::mat4 clip_transform = camera.get_view_projection() * translation;
    glUniformMatrix4fv(glGetUniformLocation(TerrainShader::ID(), "transform"), 1, GL_FALSE, &clip_transform[0][0]);

    glEnable(GL_DEPTH_TEST);
    // disable depth write for transparent surfaces
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    transparent_mesh.draw();
}

void Chunk::load_data() 
{
    unique_lock<shared_mutex> write_lock(mut);

    if (!active)
        return;
    loaded = true;
    string chunk_filename = get_chunk_filename();
    ifstream fs(chunk_filename);
    if (fs.good()) {
        try {
            ChunkIndex index;
            do {
                blocks.at(index).load(fs);
            } while (index.advance());
        } catch (SaveError& error) {
            cout << "SaveError caught: " << error.what() << endl;
            generate_data_from_seed();
        }
    } else {
        // chunk save file does not exist yet, not an error
        generate_data_from_seed();
    }
}

string Chunk::get_chunk_filename() const
{
    return "world/" +
           to_string(coords.x) + "_" + to_string(coords.y) + "_" + to_string(coords.z) +
           ".chunk";
}

void Chunk::generate_data_from_seed() {
    // grass w dirt
    if (coords.y == -1) {
        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int z = 0; z < CHUNK_WIDTH; ++z) {
                int y;
                for (y = 0; y < CHUNK_WIDTH-1; ++y) {
                    blocks.at({x,y,z}).id = BlockID::DIRT;
                }
                blocks.at({x,y,z}).id = BlockID::GRASS;
            }
        }
        return;
    }
    // beneath is 2 chunks of stone
    if (coords.y >= -3 && coords.y < -1) {
        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int y = 0; y < CHUNK_WIDTH; ++y) {
                for (int z = 0; z < CHUNK_WIDTH; ++z) {
                    blocks.at({x,y,z}).id = BlockID::STONE;
                }
            }
        }
        return;
    }
    // else air
    for (int x = 0; x < CHUNK_WIDTH; ++x) {
        for (int y = 0; y < CHUNK_WIDTH; ++y) {
            for (int z = 0; z < CHUNK_WIDTH; ++z) {
                blocks.at({x,y,z}).id = BlockID::AIR;
            }
        }
    }
}

void Chunk::save_data() 
{
    unique_lock<shared_mutex> write_lock(mut);
    
    if (!loaded)
        return;
    if (!dirty)
        return;

    dirty = false;
    string chunk_filename = get_chunk_filename();
    ofstream fs(chunk_filename);
    try {
        ChunkIndex index;
        do {
            blocks.at(index).save(fs);
        } while (index.advance());
    } catch (SaveError& error) {
        cout << "SaveError caught: " << error.what() << endl;
    }
}

void Chunk::build_meshes()
{
    shared_lock<shared_mutex> read_lock(mut);

    if (!active || !loaded)
        return;

    vector<Vertex> opaque_vertices;
    vector<unsigned> opaque_indices;
    build_opaque_vertices_and_indices(opaque_vertices, opaque_indices);
    vector<Vertex> transparent_vertices;
    vector<unsigned> transparent_indices;
    build_transarent_vertices_and_indices(transparent_vertices, transparent_indices);

    read_lock.unlock(); // this prevents a deadlock
    g_game->get_sync_queue().push(
        [this, ov = move(opaque_vertices), oi = move(opaque_indices), 
         tv = move(transparent_vertices), ti = move(transparent_indices)]
    {
        unique_lock<shared_mutex> write_lock(mut);
        if (!active || !loaded)
            return;
            
        opaque_mesh = Mesh(ov, oi);
        transparent_mesh = Mesh(tv, ti);
        can_render = true;
    });
}

void Chunk::build_opaque_vertices_and_indices(vector<Vertex>& vertices, vector<unsigned>& indices) const
{ 
    ChunkIndex index;
    do {
        const BlockData& current = blocks.at(index);
        // only render opaque blocks
        if (current.is_transparent())
            continue;
        switch (current.get_mesh_type()) {
        case BlockMesh::CUBE:
            // only add faces that are adjacent to transparent
            //      blocks, cull faces that are obscured
            for (auto& direction : directions) {
                if (should_draw_opaque_face(index, direction)) {
                    append_block_face(vertices, indices, index, current, direction.face);
                }
            }
            break;
        default:
            break;
        }
    } while (index.advance());
}

void Chunk::build_transarent_vertices_and_indices(vector<Vertex>& vertices, vector<unsigned>& indices) const
{
    ChunkIndex index;
    do {
        const BlockData& current = blocks.at(index);
        // only render opaque blocks
        if (current.is_transparent())
            continue;
        switch (current.get_mesh_type()) {
        case BlockMesh::CUBE:
            // only add faces that are adjacent to transparent
            //      blocks, cull faces that are obscured
            for (auto& direction : directions) {
                if (should_draw_transparent_face(index, direction)) {
                    append_block_face(vertices, indices, index, current, direction.face);
                }
            }
            break;
        default:
            break;
        }
    } while (index.advance());
}

bool Chunk::should_draw_opaque_face(const ChunkIndex& index, const Direction& direction) const
{
    if (int x = index.x + get<0>(direction.vec); x < 0 || x == CHUNK_WIDTH)
        return true;
    if (int y = index.y + get<1>(direction.vec); y < 0 || y == CHUNK_WIDTH)
        return true;
    if (int z = index.z + get<2>(direction.vec); z < 0 || z == CHUNK_WIDTH)
        return true;
    
    auto adj = index;
    adj += direction.vec;
    return blocks.at(adj).is_transparent();
}

bool Chunk::should_draw_transparent_face(const ChunkIndex& index, const Direction& direction) const
{
    if (int x = index.x + get<0>(direction.vec); x < 0 || x == CHUNK_WIDTH)
        return true;
    if (int y = index.y + get<1>(direction.vec); y < 0 || y == CHUNK_WIDTH)
        return true;
    if (int z = index.z + get<2>(direction.vec); z < 0 || z == CHUNK_WIDTH)
        return true;
    
    auto adj = index;
    adj += direction.vec;
    return blocks.at(adj).is_transparent() &&
           blocks.at(index).id == blocks.at(adj).id; // same material
}

void Chunk::append_block_face(vector<Vertex>& vertices, 
                       vector<unsigned int>& indices, 
                       const ChunkIndex& chunk_indices, const BlockData& current, 
                       CubeFace face) const
{
    vector<unsigned int> face_indices = current.get_face_indices(face);
    // adjust face_indices to point to the vertices we are about to add
    size_t offset = vertices.size();
    for_each(begin(face_indices), end(face_indices),
        [offset](unsigned int& index) { index += offset; }
    );
    indices.insert(end(indices), begin(face_indices), end(face_indices));

    vector<Vertex> face_vertices = current.get_face_vertices(face);
    // translate vertices of block face to position in chunk
    for_each(begin(face_vertices), end(face_vertices),
        [chunk_indices](Vertex& v) {
            v.position += glm::vec3{chunk_indices.x, chunk_indices.y, chunk_indices.z};
        }
    );
    vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());
}

BlockData Chunk::get_block(ChunkIndex indices) const
{
    shared_lock<shared_mutex> read_lock(mut);
    return blocks.at(indices);
}

void Chunk::set_block(ChunkIndex indices, BlockData block)
{
    unique_lock<shared_mutex> write_lock(mut);
    dirty = true;
    blocks.at(indices) = block;
}

bool Chunk::try_set_inactive() {
    unique_lock<shared_mutex> write_lock(mut);
    if (dirty) {
        g_game->get_thread_queue().push([this] {
            save_data();
        });
        return false;
    }
    if (pending_jobs) {
        return false;
    }
    active = false;
    loaded = false;
    can_render = false;
    return true;
}