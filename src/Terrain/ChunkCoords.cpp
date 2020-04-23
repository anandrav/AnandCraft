#include "ChunkCoords.h"

#include "ChunkWidth.h"

// Conversion from coordinate in units of blocks to units in chunks
// blocks 0 to 15 are in chunk 0
// blocks 16 to 31 are in chunk 1
// blocks -16 to -1 are in chunk -1
// and so on...
int convert_coords(int coord) { 
    if (coord >= 0)
        return coord / CHUNK_WIDTH;
    // -16 to -1 are in chunk -1
    return ((coord + 1) / CHUNK_WIDTH) - 1;
}

ChunkCoords::ChunkCoords(int x, int y, int z)
    : x(x)
    , y(y)
    , z(z)
{
}

ChunkCoords::ChunkCoords(const BlockCoords& block_coord)
    : x(convert_coords(block_coord.x))
    , y(convert_coords(block_coord.y))
    , z(convert_coords(block_coord.z))
{
}

bool ChunkCoords::operator== (const ChunkCoords& coords) const
{
    return (
        x == coords.x &&
        y == coords.y &&
        z == coords.z
    );
}