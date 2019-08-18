#pragma once

#include <unordered_map>
#include <list>
#include <math.h>

#include "glm/glm.hpp"
#include "Grid.h"

class World {
public:
    const static int RENDER_DISTANCE_IN_CHUNKS = 4;
    const int LRU_CAPACITY = pow(RENDER_DISTANCE_IN_CHUNKS, 3);

    World();

    void render_opaque(Camera& camera);

    void render_transparent(Camera& camera);

    bool has_block_at(int x, int y, int z);

    Block::State get_block_at(int x, int y, int z);

    void modify_block_at(int x, int y, int z, Block::State new_state);

    void update(glm::vec3 player_location);

private:
    Grid grid;

    struct ChunkInfo {
        bool dirty;
    };
    unordered_map<Grid::ChunkIndices, ChunkInfo, Grid::ChunkIndicesHash> chunks;
    std::list<int> LRU;
};