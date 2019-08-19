#pragma once

#include <vector>
#include <algorithm>
#include <math.h>

#include "glm/glm.hpp"
#include "BlockGrid.h"

class World {
public:
    const static int RENDER_DIST_IN_CHUNKS = 4;
    const int CHUNK_CAPACITY = (int)pow(RENDER_DIST_IN_CHUNKS, 3);
    // seed?
    // world name?
    // time of day?

    World();

    void render_opaque(Camera& camera);

    void render_transparent(Camera& camera);

    bool has_block_at(int x, int y, int z);

    Block::State get_block_at(int x, int y, int z);

    void modify_block_at(int x, int y, int z, Block::State new_state);

    void update(glm::vec3 player_location);

private:
    BlockGrid grid;
};