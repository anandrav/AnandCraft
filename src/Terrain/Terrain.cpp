#include "Terrain.h"

#include "../Physics/Ray.h"
#include "ChunkIndex.h"

#include <iostream>

using namespace std;

Terrain::Terrain(Player* player) 
    : Entity("Terrain")
    , player(player)
    , raycast_listener(ID, [this](auto e) { return this->handle_raycast_event(e); })
{
}

void Terrain::update() {
    ChunkCoords center(player->get_position());
    // remove chunks out of bounds while iterating through map
    for(auto it = begin(chunks); it != end(chunks); ) {
        int x_dist = abs(center.x - it->first.x);
        int y_dist = abs(center.y - it->first.y);
        int z_dist = abs(center.z - it->first.z);
        if (x_dist > RENDER_DIST || y_dist > RENDER_DIST || z_dist > RENDER_DIST) {
            it = chunks.erase(it);
        } else {
            ++it;
        }
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
                    auto chunk = make_shared<Chunk>(coords);
                    chunk->load_data();
                    chunk->build_meshes();
                    chunks.insert({coords, chunk});
                }
            }
        }
    }
}


bool Terrain::handle_raycast_event(shared_ptr<RaycastEvent> event) {
    cout << "RAYCAST EVENT RECEIVED BY TERRAIN" << endl;
    Ray ray(event->get_ray());
    const float MAX_DISTANCE = 32.0f;
    const float STEP_DISTANCE = 0.1f;

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