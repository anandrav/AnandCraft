#include "World.h"

World::World() : 
    grid_chunk_manager_thread(&World::grid_chunk_manager_thread_routine, this),
    is_terminating(false) {
}

World::~World() {
    is_terminating = true;
}

void World::render_opaque(Camera& camera) {
    grid.render_opaque(camera);
}

void World::render_transparent(Camera& camera) {
    grid.render_transparent(camera);
}

bool World::has_block_at(int x, int y, int z) {
    return grid.has_block(x, y, z);
}

Block::State World::get_block_at(int x, int y, int z) {
    return grid.get_block(x, y, z);
}

void World::modify_block_at(int x, int y, int z, Block::State new_state) {
    return grid.modify_block(x, y, z, new_state);
}

// update the blocks in the world in close proximity to the player
// ensure that chunks within render dist are loaded and that chunks
//      outside render dist are unloaded
void World::update(glm::vec3 player_location) {
    last_player_location = player_location;
}

// FIXME calling add_chunk and remove_chunk is not working, it fails sometimes.
// you need to make these operations thread safe or something!
void World::grid_chunk_manager_thread_routine() {
    auto data = vector<vector<vector<Block::State>>>(
        BlockGrid::CHUNK_WIDTH, vector<vector<Block::State>>(
            BlockGrid::CHUNK_HEIGHT, vector<Block::State>(
                BlockGrid::CHUNK_WIDTH, Block::State(Block::ID::AIR))));

    for (int x = 0; x < BlockGrid::CHUNK_WIDTH; ++x) {
        for (int z = 0; z < BlockGrid::CHUNK_WIDTH; ++z) {
            // 4 layers stone
            for (int y = 0; y < 10 && y < BlockGrid::CHUNK_HEIGHT; ++y) {
                data[x][y][z] = Block::State(Block::ID::STONE);
            }
            // 2 layers of dirt under grass
            for (int y = 10; y < 14 && y < BlockGrid::CHUNK_HEIGHT; ++y) {
                data[x][y][z] = Block::State(Block::ID::DIRT);
            }
            // one layer of grass on top
            data[x][14][z] = Block::State(Block::ID::GRASS);
        }
    }

    while (!is_terminating) {
        int player_x = (int)last_player_location.x;
        int player_y = (int)last_player_location.y;
        int player_z = (int)last_player_location.z;
        auto player_indices = BlockGrid::get_chunk_indices(player_x, player_y, player_z);

        std::vector<BlockGrid::ChunkIndices> must_be_loaded;
        vector<BlockGrid::ChunkIndices> already_loaded;
        for (auto it = chunk_states.begin(); it != chunk_states.end(); ++it) {
            already_loaded.push_back(it->first);
        }

        for (int x = player_indices.x - RENDER_DIST_IN_CHUNKS;
            x <= player_indices.x + RENDER_DIST_IN_CHUNKS; ++x) {
            for (int y = player_indices.y /*- RENDER_DIST_IN_CHUNKS*/;
                y <= player_indices.y /*+ RENDER_DIST_IN_CHUNKS*/; ++y) {
                for (int z = player_indices.z - RENDER_DIST_IN_CHUNKS;
                    z <= player_indices.z + RENDER_DIST_IN_CHUNKS; ++z) {
                    auto indices = BlockGrid::ChunkIndices{ x, y, z };
                    must_be_loaded.push_back(indices);

                    BlockGrid* grid_ptr = &grid;
                    if (chunk_states.find(indices) == chunk_states.end()) {
                        chunk_states[indices] = ChunkState{ false };
                        AsyncQueue::get_instance().push([grid_ptr, indices, data]() {
                                grid_ptr->add_chunk(indices, data);
                            }
                        );
                    }
                }
            }
        }

        std::sort(must_be_loaded.begin(), must_be_loaded.end());
        std::sort(already_loaded.begin(), already_loaded.end());

        std::vector<BlockGrid::ChunkIndices> must_be_evicted(already_loaded.size());
        std::vector<BlockGrid::ChunkIndices>::iterator it;
        it = std::set_difference(already_loaded.begin(), already_loaded.end(),
            must_be_loaded.begin(), must_be_loaded.end(), must_be_evicted.begin());

        must_be_evicted.resize(it - must_be_evicted.begin());

        for (int i = 0; i < must_be_evicted.size(); ++i) {
            auto indices = must_be_evicted[i];
            // todo use chunk info here
            BlockGrid* grid_ptr = &grid;
            chunk_states.erase(indices);
            AsyncQueue::get_instance().push([grid_ptr, indices]() {
                    grid_ptr->remove_chunk(indices);
                }
            );
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}