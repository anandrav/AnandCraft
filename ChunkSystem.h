/*
ChunkSystem is responsible for the chunks contained by a world.

An entity-component-system pattern is utilized to help decouple the logic for chunks, making multithreading simpler.
A chunk's ID can be used to identify its components.
Components are laid out contiguously in memory in dynamic arrays. 
This resembles data-oriented design and may boost performance by limiting cache misses, but this is not a priority.
The primary purpose of using an entity-component-system pattern is to hopefully make the logic more elegant.
All a chunk's components share the same index in their respective dynamic arrays.
The mapping of a chunk's ID to the index of its components is maintained in a std::unordered_map for fast lookup.
*/

#pragma once

#include <unordered_map>
#include <vector>
#include "WorldConfig.h"
#include "Block.h"
#include "Graphics/Mesh.h"

using ChunkID = int;
using ChunkState = BlockState[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

class ChunkSystem {
public:
    // create a chunk and return its ID
    ChunkID create_chunk();

private:
    // mapping from ID of chunk to index in the component vectors below
    std::unordered_map<ChunkID, int> index_of_ID;      

    // component vectors
    std::vector<ChunkState> state;  // block data
    std::vector<Mesh> meshes;
};

