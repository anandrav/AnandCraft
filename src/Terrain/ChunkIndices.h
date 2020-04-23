/*
 * Indices of a block within a chunk.
*/
#pragma once

#include "BlockCoords.h"

struct ChunkIndices {
    ChunkIndices(int x, int y, int z);

    // convenient conversion from block coordinates to 
    //      the indices within that block's chunk
    ChunkIndices(const BlockCoords& coords);

    int x;
    int y;
    int z;
};