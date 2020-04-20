/*
 * ChunkData.h
 * 
 * Simple container for 3-dimensional block data of a chunk.
 */

#pragma once

#include "../Block.h"
#include "ChunkWidth.h"


struct ChunkBlocks {
    BlockData blocks[CHUNK_WIDTH][CHUNK_WIDTH][CHUNK_WIDTH];
    
    BlockData& at(int x, int y, int z) 
    {
        return blocks[x][y][z];
    }

    const BlockData& at(int x, int y, int z) const
    {
        return blocks[x][y][z];
    }
};