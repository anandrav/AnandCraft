#include "ChunkSystem.h"

#include <functional>
#include <cassert>
#include "../Async/SyncQueue.h"
#include "../Async/ThreadQueue.h"

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

void ChunkSystem::generate_chunk_data(ChunkCoords coords)
{
    auto data = make_unique<ChunkData>();
    // half dirt, 1 layer of grass, the rest is air
    for (int x = 0; x < CHUNK_WIDTH; ++x) {
        for (int z = 0; z < CHUNK_WIDTH; ++z) {
            int y = 0;
            for (; y < CHUNK_WIDTH / 2; ++y) {
                 (*data)[x][y][z] = BlockData(BlockID::DIRT);
            }
            (*data)[x][y][z] = BlockData(BlockID::GRASS);
            ++y;
            for (; y < CHUNK_WIDTH; ++y) {
                (*data)[x][y][z] = BlockData(BlockID::AIR);
            }
        }
    }

    SyncQueue::get_instance().push(
        // chunk data is captured by move to prevent unnecessary copying
        [this, coords, moved_data{ move(data) }]() mutable
        {
            auto lookup = chunks.find(coords);
            if (lookup == chunks.end()) {
                // chunk doesn't exist anymore
                return;
            }
            lookup->second.chunk_data = move(moved_data);

            // TODO YOU WERE HERE
            PaddedChunkData padded_data;

            using namespace placeholders;
            ThreadQueue::get_instance().push(
                bind(&ChunkSystem::generate_chunk_meshes, this, coords, padded_data),
                ThreadQueue::Priority::NORMAL
            );
        }
    );
}

void ChunkSystem::generate_chunk_meshes(ChunkCoords coords, PaddedChunkData data)
{

}