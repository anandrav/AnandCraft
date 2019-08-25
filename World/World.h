#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <math.h>

#include "glm/glm.hpp"
#include "BlockGrid.h"


class World {
public:
    // render distance not including the chunk the character is standing on
    const static int RENDER_DIST_IN_CHUNKS = 4;

    World();

    ~World();

    void render_opaque(Camera& camera);

    void render_transparent(Camera& camera);

    bool has_block_at(int x, int y, int z);

    Block::State get_block_at(int x, int y, int z);

    void modify_block_at(int x, int y, int z, Block::State new_state);

    void update(glm::vec3 player_location);

private:
    BlockGrid grid;

    struct ChunkState {
        bool dirty = false;
    };
    unordered_map<BlockGrid::ChunkIndices, ChunkState, 
        BlockGrid::ChunkIndicesHash> chunk_states;
    glm::vec3 last_player_location;
    // thread responsible for loading and evicting chunks based on the
    //      data above
    std::thread grid_chunk_manager_thread;

    bool is_terminating;

    void grid_chunk_manager_thread_routine();
};