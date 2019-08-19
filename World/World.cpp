#include "World.h"

World::World() {
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
    auto data = vector<vector<vector<Block::State>>>(
        BlockGrid::CHUNK_WIDTH, vector<vector<Block::State>>(
            BlockGrid::CHUNK_HEIGHT, vector<Block::State>(
                BlockGrid::CHUNK_WIDTH, Block::State(Block::ID::AIR))));

    for (int x = 0; x < BlockGrid::CHUNK_WIDTH; ++x) {
        for (int z = 0; z < BlockGrid::CHUNK_WIDTH; ++z) {
            // bottom 5 layers stone
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

    int player_x = (int)player_location.x;
    int player_y = (int)player_location.y;
    int player_z = (int)player_location.z;


    std::vector<BlockGrid::ChunkIndices> must_be_loaded;
    must_be_loaded.reserve(CHUNK_CAPACITY);

    auto player_indices = BlockGrid::get_chunk_indices(player_x, player_y, player_z);
    //std::cout << "PLAYER LOCATION: " << player_indices.x << ' ' << player_indices.y << ' ' << player_indices.z << std::endl;

    for (int x = player_indices.x - RENDER_DIST_IN_CHUNKS;
        x < player_indices.x + RENDER_DIST_IN_CHUNKS; ++x) {
        for (int y = player_indices.y /*RENDER_DIST_IN_CHUNKS*/;
            y < player_indices.y + 1/*+ RENDER_DIST_IN_CHUNKS*/; ++y) {
            for (int z = player_indices.z - RENDER_DIST_IN_CHUNKS;
                z < player_indices.z + RENDER_DIST_IN_CHUNKS; ++z) {
                auto chunk_indices = BlockGrid::ChunkIndices{ x, y, z };
                must_be_loaded.push_back(chunk_indices);

                // THS IS AN URGENT ISSUE
                // FIXME this isn't a good check because if you've already tried
                // to add a chunk but it hasn't gone through yet, you'll
                // do it again and again and again!
                if (!grid.has_chunk(chunk_indices)) {
                    grid.add_chunk(chunk_indices, data);
                }
            }
        }
    }

    //auto already_loaded = grid.get_loaded_chunks();

    //std::sort(must_be_loaded.begin(), must_be_loaded.end());
    //std::sort(already_loaded.begin(), already_loaded.end());

    //std::vector<BlockGrid::ChunkIndices> must_be_evicted(already_loaded.size());
    //std::vector<BlockGrid::ChunkIndices>::iterator it;
    //it = std::set_difference(already_loaded.begin(), already_loaded.end(),
    //    must_be_loaded.begin(), must_be_loaded.end(), must_be_evicted.begin());

    //must_be_evicted.resize(it - must_be_evicted.begin());
}