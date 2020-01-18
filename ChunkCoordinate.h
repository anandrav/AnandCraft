/*
Coordinate in World space in units of Chunks.
Used for identifying chunks by location.
*/

#pragma once

#include "BlockCoordinate.h"

struct ChunkCoordinate {
    explicit ChunkCoordinate(BlockCoordinate block_coord);

    int x;
    int y;
    int z;
};