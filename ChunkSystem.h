/*
ChunkSystem maintains the chunks used by a World, and provides an interface for managing them.

An entity-component-system pattern is utilized to help decouple the logic for chunks, making multithreading simpler.
A chunk's coordinates are used to identify its components.
Components are laid out contiguously in memory in dynamic arrays.
This resembles data-oriented design and may boost performance by limiting cache misses, but this is not a priority.
The primary purpose of using an entity-component-system pattern is to hopefully make the logic more elegant.
All a chunk's components share the same index in their respective dynamic arrays.
The mapping of a chunk's ID to the index of its components is maintained in a std::unordered_map for fast lookup.
*/

#pragma once

#include <unordered_map>
#include <vector>
#include <array>
#include "WorldConfig.h"
#include "ChunkCoords.h"
#include "Block.h"
#include "Graphics/Mesh.h"

// chunk data is a simple 3D array of blocks
using ChunkState = std::array<std::array<std::array<BlockState, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE>;

struct ChunkCoordsHash {
    std::size_t operator() (const ChunkCoords& coords) const;
};

class ChunkSystem {
public:
    void create_chunk(ChunkCoords coords);

    void remove_chunk(ChunkCoords coords);

    // return the number of chunks in the ChunkSystem
    size_t num_chunks();

private:
    // mapping from chunk coords to index in the component vectors below
    std::unordered_map<ChunkCoords, size_t, ChunkCoordsHash> indices;

    // component vectors
    std::vector<ChunkState> chunk_states;
    std::vector<Mesh> chunk_meshes;
    std::vector<ChunkCoords> chunk_coords;  //mapping from index to chunk coords
};