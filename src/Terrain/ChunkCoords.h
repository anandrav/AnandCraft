/*
 * Coordinate in World space in units of Chunks.
 * Used for identifying chunks by location.
 */

#pragma once

#include "BlockCoords.h"

struct ChunkCoords {
    ChunkCoords(int x, int y, int z);

    // convenient conversion from BlockCoordinates to ChunkCoordinates
    ChunkCoords(const BlockCoords& block_coord);

    ChunkCoords(const ChunkCoords& other) = default;

    bool operator== (const ChunkCoords& coords) const;

    int x;
    int y;
    int z;
};