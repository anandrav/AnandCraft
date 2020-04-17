/*
 * ChunkData.h
 * 
 * Simple container for 3-dimensional block data of a chunk.
 */

#pragma once

#include "WorldConfig.h"
#include "../Block.h"

struct ChunkData {
    BlockData blocks[CHUNK_WIDTH][CHUNK_WIDTH][CHUNK_WIDTH];

    BlockData& at(size_t x, size_t y, size_t z) 
    {
        return blocks[x][y][z];
    }

    const BlockData& at(size_t x, size_t y, size_t z) const
    {
        return blocks[x][y][z];
    }
};