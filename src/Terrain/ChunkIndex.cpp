#include "ChunkIndex.h"

#include "ChunkWidth.h"

int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

int convert_to_index(int coord) { 
    return positive_modulo(coord, CHUNK_WIDTH);
}

ChunkIndex::ChunkIndex()
    : x(0)
    , y(0)
    , z(0)
    {
    }

ChunkIndex::ChunkIndex(int x, int y, int z)
    : x(convert_to_index(x))
    , y(convert_to_index(y))
    , z(convert_to_index(z))
    {
    }

ChunkIndex::ChunkIndex(const BlockCoords& coords)
    : x(convert_to_index(coords.x))
    , y(convert_to_index(coords.y))
    , z(convert_to_index(coords.z))
    {
    }