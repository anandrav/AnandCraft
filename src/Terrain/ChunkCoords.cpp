#include "ChunkCoords.h"

#include "ChunkWidth.h"

// Conversion from coordinate in units of blocks to units in chunks
// blocks 0 to 15 are in chunk 0
// blocks 16 to 31 are in chunk 1
// blocks -16 to -1 are in chunk -1
// and so on...
// must account for rounding of negative numbers!
int block_to_chunk_coord(int element) { 
    if (element >= 0) {
        return element / CHUNK_WIDTH;
    }
    // negative number rounding case
    return ((element + 1) / CHUNK_WIDTH) - 1;
}

ChunkCoords::ChunkCoords(int x, int y, int z)
    : x(x)
    , y(y)
    , z(z)
{
}

ChunkCoords::ChunkCoords(const BlockCoords& block_coord)
    : x(block_to_chunk_coord(block_coord.x))
    , y(block_to_chunk_coord(block_coord.y))
    , z(block_to_chunk_coord(block_coord.z))
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