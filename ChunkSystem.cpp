#include "ChunkSystem.h"

#include <functional>   // for std::hash
#include <cassert>

using namespace std;

size_t ChunkCoordsHash::operator() (const ChunkCoords& coords) const
{
    size_t h1 = hash<int>()(coords.x);
    size_t h2 = hash<int>()(coords.y);
    size_t h3 = hash<int>()(coords.z);

    return h1 ^ (h2 << 1) ^ (h3 << 2);
}

void ChunkSystem::create_chunk(ChunkCoords coords)
{
    // create new chunk's components
    ChunkState state;
    Mesh mesh;

    // add new chunk's components to the end of the component arrays
    chunk_states.push_back(state);
    chunk_meshes.push_back(move(mesh));

    // update mapping from coordinates to indices
    indices.insert({coords, chunk_states.size()-1});
}

void ChunkSystem::remove_chunk(ChunkCoords coords)
{
    auto lookup = indices.find(coords);
    if (lookup == indices.end()) {
        throw exception("No chunk at those coordinates!");
    }

    size_t index = lookup->second;

    // if chunk at index is not the last chunk
    if (index != chunk_states.size() - 1) {
        // swap chunk at index with last chunk
        // last chunk is now a "victim"
        indices[chunk_coords.back()] = index;
        swap(chunk_states[index], chunk_states.back());
        swap(chunk_meshes[index], chunk_meshes.back());
        swap(chunk_coords[index], chunk_coords.back());
    }
    // remove last chunk
    indices.erase(coords);
    chunk_states.pop_back();
    chunk_meshes.pop_back();
    chunk_coords.pop_back();
}

size_t ChunkSystem::num_chunks()
{
    // component arrays should not vary in length
    assert(chunk_states.size() == chunk_meshes.size());

    return chunk_states.size();
}