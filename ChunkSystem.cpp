#include "ChunkSystem.h"

#include <functional>   // for std::hash
#include <cassert>

using namespace std;

size_t ChunkSystem::ChunkCoordsHash::operator() (const ChunkCoords& coords) const
{
    size_t h1 = hash<int>()(coords.x);
    size_t h2 = hash<int>()(coords.y);
    size_t h3 = hash<int>()(coords.z);

    return h1 ^ (h2 << 1) ^ (h3 << 2);
}

void ChunkSystem::create_chunk(ChunkCoords coords)
{
    chunks.insert({ coords, ChunkComponents{coords} });
}

void ChunkSystem::remove_chunk(ChunkCoords coords)
{
    chunks.erase(coords);
}

size_t ChunkSystem::num_chunks() const
{
    chunks.size();
}