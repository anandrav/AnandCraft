#include "ChunkCoords.h"

#include "WorldConfig.h"

// Conversion from coordinate in units of blocks to units in chunks
// blocks 0 to 15 are in chunk 0
// blocks 16 to 31 are in chunk 1
// blocks -16 to -1 are in chunk -1
// and so on...
// must account for rounding of negative numbers!
constexpr int block_to_chunk_coord(int element) { 
    if (element >= 0) {
        return element / CHUNK_SIZE;
    }
    // negative number rounding case
    return ((element + 1) / CHUNK_SIZE) - 1;
}

ChunkCoords::ChunkCoords(int x_, int y_, int z_)
    : x(x_)
    , y(y_)
    , z(z_)
{
}

ChunkCoords::ChunkCoords(BlockCoords block_coord)
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