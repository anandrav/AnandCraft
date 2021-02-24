/*
 * Indices of a block within a chunk.
*/
#pragma once

#include "ChunkWidth.h"
#include "BlockCoords.h"
#include <tuple>

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

    // iterate through (x,y,z) coordinates
    // like a triple-nested for loop
    template <bool X, bool Y, bool Z>
    bool advance_impl();

    bool advance() {
        return advance_impl<true,true,true>();
    }

    bool advance_xy() {
        return advance_impl<true,true,false>();
    }

    bool advance_xz() {
        return advance_impl<true,false,true>();
    }

    bool advance_yz() {
        return advance_impl<false,true,true>();
    }

    bool out_of_bounds() const;

    int x;
    int y;
    int z;
};

// iterate through (x,y,z) coordinates as specified by template params
// like a nested for loop
template<bool X, bool Y, bool Z>
bool ChunkIndex::advance_impl() {
    // innermost loop
    if constexpr (Z) {
        if (z < CHUNK_WIDTH-1) {
            ++z;
            return true;
        }
        z = 0;
    }
    if constexpr (Y) {
        // middle loop
        if (y < CHUNK_WIDTH-1) {
            ++y;
            return true;
        }
        y = 0;
    }
    if constexpr (X) {
        // outermost loop
        if (x < CHUNK_WIDTH-1) {
            ++x;
            return true;
        }
        x = 0;
    }
    return false;
}