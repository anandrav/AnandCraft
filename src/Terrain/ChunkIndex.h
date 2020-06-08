/*
 * Indices of a block within a chunk.
*/
#pragma once

#include "BlockCoords.h"

struct ChunkIndex {
    // initialize with zeroes
    ChunkIndex();

    ChunkIndex(int x, int y, int z);

    // convenient conversion from block coordinates to 
    //      the indices within that block's chunk
    explicit ChunkIndex(const BlockCoords& coords);

    ChunkIndex& operator+=(const std::tuple<int,int,int>& vec) {
        x += std::get<0>(vec);
        y += std::get<1>(vec);
        z += std::get<2>(vec);
        return *this;
    }

    // iterate through all (x,y,z) coordinates
    // like a triple-nested for loop
    bool advance();

    bool out_of_bounds() const;

    int x;
    int y;
    int z;
};