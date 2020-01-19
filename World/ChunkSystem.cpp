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

void fill_padded_chunk_data_with_adjacent_chunk(ChunkFace face, const ChunkSystem* chunk_system, ChunkCoords coords, PaddedChunkData& data);

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

void ChunkSystem::GenerateChunkJob::operator()()
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

    /* todo ensure everything below is on the main thread, even if Job is on another thread */

    auto lookup = chunk_system->chunks.find(chunk_coords);
    if (lookup == chunk_system->chunks.end()) {
        // chunk no longer exists
        return;
    }

    lookup->second.data = data;
}

ChunkSystem::UpdateChunkMeshesJob::UpdateChunkMeshesJob(ChunkSystem* chunk_system_, ChunkCoords chunk_coords_)
    : chunk_system(chunk_system_), chunk_coords(chunk_coords_)
{
    fill_padded_chunk_data_with_adjacent_chunk(ChunkFace::XNEG, chunk_system, chunk_coords, padded_chunk_data);
}

void ChunkSystem::UpdateChunkMeshesJob::operator()()
{

}