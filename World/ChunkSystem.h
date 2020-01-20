/*
ChunkSystem maintains the chunks used by a World, and provides an interface for managing them.

Chunks are stored and accessed using their coordinates.
*/

#pragma once

#include <unordered_map>
#include <array>
#include <memory>
#include "WorldConfig.h"
#include "ChunkCoords.h"
#include "../Block.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Renderable.h"

//using ChunkData = BlockData[CHUNK_WIDTH][CHUNK_WIDTH][CHUNK_WIDTH];
//using PaddedChunkData = BlockData[CHUNK_WIDTH+2][CHUNK_WIDTH+2][CHUNK_WIDTH+2];
using ChunkData = std::array<std::array<std::array<BlockData, CHUNK_WIDTH>, CHUNK_WIDTH>, CHUNK_WIDTH>;
using PaddedChunkData = std::array<std::array<std::array<BlockData, CHUNK_WIDTH + 2>, CHUNK_WIDTH + 2>, CHUNK_WIDTH + 2>;

class ChunkSystem : public Renderable {
public:
    void create_chunk(ChunkCoords coords);

    void remove_chunk(ChunkCoords coords);

    void set_block_data(BlockCoords coords, BlockData data);

    BlockData get_block_data(BlockCoords coords);

    // return the number of chunks in the ChunkSystem
    size_t num_chunks() const;

    void render_opaque() const override;

    void render_transparent() const override;

private:
    // Generate chunk data
    // Can be run off the main thread
    void generate_chunk_data(ChunkCoords coords);

    // generating chunk meshes requires knowledge of adjacent blocks
    // so we pad by one block on either side
    using PaddedChunkData = std::vector<std::vector<std::vector<BlockData>>>;

    // Update a chunk's meshes, opaque and transparent.
    // Can be run off the main thread
    void generate_chunk_meshes(ChunkCoords coords, PaddedChunkData);

    // All the information about a chunk is stored in ChunkComponents
    struct ChunkComponents {
        // even an uninitialized chunk must have coordinates
        ChunkComponents(ChunkCoords coords_)
            : coords(coords_)
        {
        }

        std::unique_ptr<ChunkData> chunk_data;
        Mesh opaque_mesh;
        Mesh transparent_mesh;
        ChunkCoords coords;
    };

    // Hash Function
    struct ChunkCoordsHash {
        std::size_t operator() (const ChunkCoords& coords) const;
    };

    // mapping from chunk coords to chunk components
    std::unordered_map<ChunkCoords, ChunkComponents, ChunkCoordsHash> chunks;
};