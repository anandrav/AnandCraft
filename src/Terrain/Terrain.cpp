#include "Terrain.h"

#include <iostream>

using namespace std;

Terrain::Terrain(Player* player) 
    : player(player)
{
    ChunkCoords coords(0,-1,0);
    chunks[coords] = make_shared<Chunk>(coords);
    coords.x = -1;
    chunks[coords] = make_shared<Chunk>(coords);
    coords.z = -1;
    chunks[coords] = make_shared<Chunk>(coords);
    coords.x = 0;
    chunks[coords] = make_shared<Chunk>(coords);
    for_each(begin(chunks), end(chunks),
        [](auto& it) {
            it.second->build_meshes();
        }
    );
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
    for (int x_offset = -RENDER_DIST; x_offset <= RENDER_DIST; ++x_offset) {
        for (int y_offset = -RENDER_DIST; y_offset < 0; ++y_offset) {
            for (int z_offset = -RENDER_DIST; z_offset <= RENDER_DIST; ++z_offset) {
                int x = center.x + x_offset;
                int y = center.y + y_offset;
                int z = center.z + z_offset;
                ChunkCoords coords(x, y, z);
                auto it = chunks.find(coords);
                if (it == chunks.end()) {
                    auto chunk = make_shared<Chunk>(coords);
                    chunk->build_meshes();
                    chunks.insert({coords, chunk});
                }
            }
        }
    }
}