#include "ChunkCoordinate.h"

#include "WorldConfig.h"

// blocks 0 to 15 are in chunk 0
// blocks 16 to 31 are in chunk 1
// blocks -16 to -1 are in chunk -1
// and so on...
// must account for rounding of negative numbers!
constexpr int block_to_chunk(int element) { 
    if (element >= 0) {
        return element / CHUNK_SIZE;
    }
    // negative number case
    return ((element + 1) / CHUNK_SIZE) - 1;
}

ChunkCoordinate::ChunkCoordinate(BlockCoordinate block_coord)
    : x(block_to_chunk(block_coord.x))
    , y(block_to_chunk(block_coord.y))
    , z(block_to_chunk(block_coord.z))
{
}