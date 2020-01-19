/*
ChunkSystem maintains the chunks used by a World, and provides an interface for managing them.

Chunks are stored and accessed using their coordinates.
*/

#pragma once

#include <unordered_map>
#include <vector>
#include <array>
#include "WorldConfig.h"
#include "ChunkCoords.h"
#include "../Block.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Renderable.h"

// chunk data is a simple 3D array of blocks
using ChunkBlocks = std::array<std::array<std::array<BlockState, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE>;

class ChunkSystem : public Renderable {
public:
    void create_chunk(ChunkCoords coords);

    void remove_chunk(ChunkCoords coords);

    // return the number of chunks in the ChunkSystem
    size_t num_chunks() const;

    void render_opaque() const override;

    void render_transparent() const override;

private:
    struct ChunkComponents {
        // even an uninitialized chunk must have coordinates
        ChunkComponents(ChunkCoords coords_)
            : blocks_initialized(false), coords(coords_)
        {
        }
        
        // flag indicating whether blocks been initialized
        bool blocks_initialized;
        ChunkBlocks blocks;
        Mesh opaque_mesh;
        Mesh transparent_mesh;
        /* Even though we have a map from coords to components below, it is useful to
        keep track of coords within the struct as well. */
        ChunkCoords coords;
    };

    struct ChunkCoordsHash {
        std::size_t operator() (const ChunkCoords& coords) const;
    };

    // mapping from chunk coords to chunk components
    std::unordered_map<ChunkCoords, ChunkComponents, ChunkCoordsHash> chunks;
};