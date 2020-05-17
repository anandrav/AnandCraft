#include "Terrain.h"

#include "../Physics/Ray.h"
#include "ChunkIndex.h"

#include "../SaveError.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>

using namespace std;

const int RENDER_DIST = 5;

Terrain::Terrain(Player* player) 
    : Entity("Terrain")
    , player(player)
    , raycast_listener(ID, [this](auto e) { return this->handle_raycast_event(e); })
{
    // create world folder
    if (mkdir("world/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
        if (errno != EEXIST) {
            throw SaveError("could not create world folder");
        }
    }

    // fill chunk pool
    int diameter = 1 + RENDER_DIST * 2;
    int num_chunks = diameter * diameter * diameter;
    for (int i = 0; i < num_chunks; ++i) {
        chunk_pool.push(make_shared<Chunk>());
    }
}

Terrain::~Terrain() {
    while (!chunks.empty()) {
        auto it = chunks.begin();
        auto chunk = it->second;
        if (chunk->try_set_inactive()) {
            it = chunks.erase(it);
            chunk_pool.push(chunk);
        }
    }

    cout << "terminated!" << endl;
}

void Terrain::update() {
    ChunkCoords center(player->get_position());
    // remove chunks out of bounds while iterating through map
    for(auto it = begin(chunks); it != end(chunks); ) {
        int x_dist = abs(center.x - it->first.x);
        int y_dist = abs(center.y - it->first.y);
        int z_dist = abs(center.z - it->first.z);
        if (x_dist > RENDER_DIST || y_dist > RENDER_DIST || z_dist > RENDER_DIST) {
            auto chunk = it->second;
            if (chunk->try_set_inactive()) {
                it = chunks.erase(it);
                chunk_pool.push(chunk);
                continue;
            }
        }
        ++it;
    }
    // add chunks in bounds if not already in map
    for (int x_offset = -RENDER_DIST; x_offset <= RENDER_DIST; ++x_offset) {
        for (int y_offset = -RENDER_DIST; y_offset <= RENDER_DIST; ++y_offset) {
            for (int z_offset = -RENDER_DIST; z_offset <= RENDER_DIST; ++z_offset) {
                int x = center.x + x_offset;
                int y = center.y + y_offset;
                int z = center.z + z_offset;
                ChunkCoords coords(x, y, z);
                auto it = chunks.find(coords);
                if (it == chunks.end()) {
                    if (chunk_pool.empty()) {
                        cout << "no chunks available right now" << endl;
                        continue;
                    }
                    auto chunk = chunk_pool.top();
                    chunk_pool.pop();
                    chunk->set_active(coords);
                    chunks.insert({coords, chunk});
                    g_game->get_thread_queue().push([chunk] {
                        chunk->load_data();
                        g_game->get_thread_queue().push([chunk] {
                            chunk->build_meshes();
                        });
                    });
                }
            }
        }
    }
}

void Terrain::render_opaque(const Camera& camera) const
{
    for (auto pair : chunks) {
        pair.second->render_opaque(camera);
    } 
}

void Terrain::render_transparent(const Camera& camera) const
{
    for (auto pair : chunks) {
        pair.second->render_transparent(camera);
    } 
}

bool Terrain::handle_raycast_event(shared_ptr<RaycastEvent> event) {
    cout << "RAYCAST EVENT RECEIVED BY TERRAIN" << endl;
    Ray ray(event->get_ray());
    const float MAX_DISTANCE = 12.0f;
    const float STEP_DISTANCE = 0.5f;

    BlockCoords p_block_coords(ray.get_end());
    cout << "raycast start: " << p_block_coords << endl;
    shared_ptr<Chunk> chunk;
    auto it = chunks.find(p_block_coords);
    if (it == end(chunks)) {
        cout << "raycast detected non-existent chunk!" << endl;
        return false;
    }
    chunk = it->second;
    // find the first block that isn't air and do something
    for (; ray.get_length() < MAX_DISTANCE; ray.step(STEP_DISTANCE)) {
        bool crossed_chunks = false;
        BlockCoords curr_block(ray.get_end());
        // don't check the same block twice
        if (curr_block == p_block_coords)
            continue;
        shared_ptr<Chunk> p_chunk;
        if (ChunkCoords(curr_block) != ChunkCoords(p_block_coords)) {
            crossed_chunks = true;
            auto it = chunks.find(curr_block);
            if (it == end(chunks)) {
                cout << "raycast detected non-existent chunk!" << endl;
                return false;
            }
            p_chunk = chunk;
            chunk = it->second;
        }
        BlockData block_data = chunk->get_block(curr_block);
        cout << "Block: " << block_data.get_name() << endl;
        if (block_data.id != BlockID::AIR) {
            if (event->is_left_click()) {
                // break block
                chunk->set_block(curr_block, {BlockID::AIR});
                chunk->build_meshes();
            } else {
                // place block
                if (crossed_chunks) {
                    p_chunk->set_block(p_block_coords, {BlockID::COBBLESTONE});
                    p_chunk->build_meshes();
                } else {
                    chunk->set_block(p_block_coords, {BlockID::COBBLESTONE});
                    chunk->build_meshes();
                }
            }
            return true;
        }
        p_block_coords = curr_block;
    }

    return false;
}