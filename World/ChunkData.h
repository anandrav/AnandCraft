/*
ChunkData is a 3D array containing the block data for a chunk.
Internally, the data is stored in a single array to take advantage of cache locality.
The length of this array is CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH

PaddedChunkData is slightly larger to store an additional block on each side.
The length of its array is (CHUNK_WIDTH + 2) + (CHUNK_WIDTH + 2) + (CHUNK_WIDTH + 2).
*/

#pragma once

#include "WorldConfig.h"
#include "../Block.h"

constexpr int PADDED_CHUNK_WIDTH = CHUNK_WIDTH + 2;

template<int WIDTH>
class ChunkDataImplementation {
public:
    BlockData& operator()(int x, int y, int z)
    {
        int index = 0;
        index += x * CHUNK_WIDTH * CHUNK_WIDTH;
        index += y * CHUNK_WIDTH;
        index += z;

        return data[index];
    }

    // TODO YOU WERE HERE

private:
    BlockData* data;
};

using ChunkData = ChunkDataImplementation<CHUNK_WIDTH>;
using PaddedChunkData = ChunkDataImplementation<PADDED_CHUNK_WIDTH>;