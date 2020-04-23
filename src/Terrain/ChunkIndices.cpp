#include "ChunkIndices.h"

#include "ChunkWidth.h"

int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

int convert_to_index(int coord) { 
    return positive_modulo(coord, CHUNK_WIDTH);
}

ChunkIndices::ChunkIndices(int x, int y, int z)
    : x(convert_to_index(x))
    , y(convert_to_index(y))
    , z(convert_to_index(z))
    {
    }

ChunkIndices::ChunkIndices(const BlockCoords& coords)
    : x(convert_to_index(coords.x))
    , y(convert_to_index(coords.y))
    , z(convert_to_index(coords.z))
    {
    }