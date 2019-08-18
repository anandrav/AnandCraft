#include "World.h"

World::World() {
    auto data = vector<vector<vector<Block::State>>>(
        Grid::CHUNK_WIDTH, vector<vector<Block::State>>(
        Grid::CHUNK_HEIGHT, vector<Block::State>(
        Grid::CHUNK_WIDTH, Block::State(Block::ID::AIR))));

    for (int x = 0; x < Grid::CHUNK_WIDTH; ++x) {
        for (int z = 0; z < Grid::CHUNK_WIDTH; ++z) {
            // bottom 5 layers stone
            for (int y = 0; y < 5 && y < Grid::CHUNK_HEIGHT; ++y) {
                data[x][y][z] = Block::State(Block::ID::STONE);
            }
            // 2 layers of dirt under grass
            for (int y = 5; y < 7 && y < Grid::CHUNK_HEIGHT; ++y) {
                data[x][y][z] = Block::State(Block::ID::DIRT);
            }
            // one layer of grass on top
            data[x][7][z] = Block::State(Block::ID::GRASS);
        }
    }

    const int chunk_radius = 5;
    for (int x = -1*chunk_radius; x < chunk_radius; ++x) {
        for (int z = -1*chunk_radius; z < chunk_radius; ++z) {
            for (int y = -4; y < 0; ++y) {
                grid.add_chunk(x,y,z,data);
            }
        }
    }
}

void World::render_opaque(Camera& camera) {
    grid.render_opaque(camera);
}

void World::render_transparent(Camera& camera) {
    grid.render_transparent(camera);
}

bool World::has_block_at(int x, int y, int z) {
    return grid.has_block_at(x, y, z);
}

Block::State World::get_block_at(int x, int y, int z) {
    return grid.get_block_at(x, y, z);
}

void World::modify_block_at(int x, int y, int z, Block::State new_state) {
    return grid.modify_block_at(x, y, z, new_state);
}

void World::update(glm::vec3 player_location) {
    
}