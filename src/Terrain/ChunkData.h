/*
 * ChunkData.h
 * 
 * Simple container for 3-dimensional block data of a chunk.
 */

#pragma once

#include "../Block.h"
#include "ChunkWidth.h"
#include "ChunkIndex.h"

struct ChunkBlocks {
    BlockData blocks[CHUNK_WIDTH][CHUNK_WIDTH][CHUNK_WIDTH];
    
    BlockData& at(ChunkIndex indices) 
    {
        auto [x, y, z] = indices;
        return blocks[x][y][z];
    }

    const BlockData& at(ChunkIndex indices) const
    {
        auto [x, y, z] = indices;
        return blocks[x][y][z];
    }
};