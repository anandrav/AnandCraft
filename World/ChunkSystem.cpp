#include "ChunkSystem.h"

#include <functional>
#include <cassert>

using namespace std;

enum class ChunkFace {
    XPOS,
    XNEG,
    YPOS,
    YNEG,
    ZPOS,
    ZNEG
};

size_t ChunkSystem::ChunkCoordsHash::operator() (const ChunkCoords& coords) const
{
    size_t h1 = hash<int>()(coords.x);
    size_t h2 = hash<int>()(coords.y);
    size_t h3 = hash<int>()(coords.z);

    return h1 ^ (h2 << 1) ^ (h3 << 2);
}

void ChunkSystem::create_chunk(ChunkCoords coords)
{
    chunks.insert({ coords, ChunkComponents{coords} });


}

void ChunkSystem::remove_chunk(ChunkCoords coords)
{
    chunks.erase(coords);
}

size_t ChunkSystem::num_chunks() const
{
    return chunks.size();
}

ChunkData ChunkSystem::generate_chunk_data(ChunkCoords coords)
{
    ChunkData data;
    // half dirt, 1 layer of grass, the rest is air
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            int y = 0;
            for (; y < CHUNK_SIZE / 2; ++y) {
                data[x][y][z] = BlockData(BlockID::DIRT);
            }
            data[x][y][z] = BlockData(BlockID::GRASS);
            ++y;
            for (; y < CHUNK_SIZE; ++y) {
                data[x][y][z] = BlockData(BlockID::AIR);
            }
        }
    }

    return data;
}