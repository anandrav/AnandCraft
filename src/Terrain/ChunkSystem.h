// /*
// ChunkSystem maintains the chunks used by a World, and provides an interface for managing them.

// Chunks are stored and accessed using their coordinates.
// */

// #pragma once

// #include <unordered_map>
// #include <array>
// #include <memory>
// #include "WorldConfig.h"
// #include "ChunkCoords.h"
// #include "../Block.h"
// #include "../Graphics/Mesh.h"
// #include "../Graphics/Renderable.h"

// //using ChunkData = BlockData[CHUNK_WIDTH][CHUNK_WIDTH][CHUNK_WIDTH];
// //using PaddedChunkData = BlockData[CHUNK_WIDTH+2][CHUNK_WIDTH+2][CHUNK_WIDTH+2];
// using ChunkData = std::array<std::array<std::array<BlockData, CHUNK_WIDTH>, CHUNK_WIDTH>, CHUNK_WIDTH>;
// using PaddedChunkData = std::array<std::array<std::array<BlockData, CHUNK_WIDTH + 2>, CHUNK_WIDTH + 2>, CHUNK_WIDTH + 2>;

// class ChunkSystem : public Renderable {
// public:
//     void create_chunk(ChunkCoords coords);

//     void remove_chunk(ChunkCoords coords);

//     void set_block_data(BlockCoords coords, BlockData data);

//     BlockData get_block_data(BlockCoords coords);

//     // return the number of chunks in the ChunkSystem
//     size_t num_chunks() const;

//     void render_opaque(const Camera& camera) const override;

//     void render_transparent(const Camera& camera) const override;

// private:
//     // All the information about a chunk is stored in ChunkComponents
//     struct ChunkComponents {
//         // even an uninitialized chunk must have coordinates
//         ChunkComponents(ChunkCoords coords_)
//             : coords(coords_)
//         {
//         }

//         ChunkComponents(const ChunkComponents&) = default;

//         // ChunkData* chunk_data;
//         Mesh opaque_mesh;
//         Mesh transparent_mesh;
//         ChunkCoords coords;
//     };

//     // Generate chunk data
//     // Can be run off the main thread
//     void generate_chunk_data(ChunkCoords coords);

//     // Make padded chunk data using chunk located at coords and its adjacent chunks.
//     // If no adjacent chunk can be found for a certain face, fill data with BlockID::AIR.
//     // PaddedChunkData is used to generate chunk meshes on another thread, because generating
//     // chunk mehses requires knowledge of adjacent chunks.
//     PaddedChunkData* make_padded_chunk_data(const ChunkComponents& components) const;

//     void copy_data_from_adjacent_chunk(PaddedChunkData* padded, ChunkCoords coords, CubeFace face) const;

//     // Update a chunk's meshes, opaque and transparent.
//     // Can be run off the main thread
//     void generate_chunk_meshes(ChunkCoords coords, const PaddedChunkData* const padded_data);

//     // Hash function for ChunkCoords
//     struct ChunkCoordsHash {
//         std::size_t operator() (const ChunkCoords& coords) const;
//     };

//     // mapping from chunk coords to chunk components
//     std::unordered_map<ChunkCoords, ChunkComponents, ChunkCoordsHash> chunks;
// };