#include "Chunk.h"

#include "WorldConfig.h"
#include "TerrainShader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

using namespace std;

Chunk::Chunk(ChunkCoords coords)
: coords(coords)
{
    translation = glm::translate(glm::vec3{coords.x*CHUNK_WIDTH, coords.y*CHUNK_WIDTH, coords.z*CHUNK_WIDTH});
}

void Chunk::render_opaque(const Camera& camera) const
{
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

void Chunk::update_meshes()
{
    update_opaque_mesh();
}

void Chunk::update_opaque_mesh() 
{
    vector<Vertex> vertices;
    vector<unsigned> indices;
    for (int x = 0; x < CHUNK_WIDTH; ++x) {
        for (int y = 0; y < CHUNK_WIDTH; ++y) {
            for (int z = 0; z < CHUNK_WIDTH; ++z) {
                BlockData& current = blocks.at(x,y,z);
                // only render opaque blocks
                if (current.is_transparent())
                    continue;
                switch (current.get_mesh_type()) {
                case BlockMesh::CUBE:
                    // only add faces that are adjacent to transparent
                    //      blocks, cull faces that are obscured
                    if (is_transparent_at(x - 1, y, z)) {
                        append_block_face(vertices, indices, x, y, z, current,
                            CubeFace::XNEG);
                    }
                    if (is_transparent_at(x + 1, y, z)) {
                        append_block_face(vertices, indices, x, y, z, current,
                            CubeFace::XPOS);
                    }
                    if (is_transparent_at(x, y - 1, z)) {
                        append_block_face(vertices, indices, x, y, z, current,
                            CubeFace::YNEG);
                    }
                    if (is_transparent_at(x, y + 1, z)) {
                        append_block_face(vertices, indices, x, y, z, current,
                            CubeFace::YPOS);
                    }
                    if (is_transparent_at(x, y, z - 1)) {
                        append_block_face(vertices, indices, x, y, z, current,
                            CubeFace::ZNEG);
                    }
                    if (is_transparent_at(x, y, z + 1)) {
                        append_block_face(vertices, indices, x, y, z, current,
                            CubeFace::ZPOS);
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    opaque_mesh = Mesh(vertices, indices);
}

void Chunk::append_block_face(vector<Vertex>& vertices, 
                       vector<unsigned int>& indices, 
                       int x, int y, int z, const BlockData& block, 
                       CubeFace face) const 
{
    vector<unsigned int> face_indices = block.get_face_indices(face);
    // adjust face_indices to point to the vertices we are about to add
    size_t offset = vertices.size();
    for_each(begin(face_indices), end(face_indices),
        [offset](unsigned int& index) { index += offset; }
    );
    indices.insert(end(indices), begin(face_indices), end(face_indices));

    vector<Vertex> face_vertices = block.get_face_vertices(face);
    // translate vertices of block face to position in chunk
    for_each(begin(face_vertices), end(face_vertices),
        [x,y,z](Vertex& v) {
            v.position += glm::vec3{x,y,z};
        }
    );
    vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());
}

bool Chunk::is_transparent_at(int x, int y, int z) const 
{
    if (x < 0 || y < 0 || z < 0)
        return true;
    if (x == CHUNK_WIDTH || y == CHUNK_WIDTH || z == CHUNK_WIDTH)
        return true;
    return blocks.at(x, y, z).is_transparent();
}