/*
 * Chunk.h
 * 
 * Chunk of blocks.
 */

#pragma once

#include "ChunkCoords.h"
#include "ChunkData.h"
#include "../Entity.h"
#include "../Graphics/Mesh.h"
#include "ChunkIndex.h"

#include <shared_mutex>
#include <memory>

struct Direction;

class Chunk : public std::enable_shared_from_this<Chunk> {
public:
    Chunk();

    Chunk(Chunk&) = delete;
    Chunk& operator=(Chunk&) = delete;

    // initializes Chunk with coords and sends load job and build job to thread queue
    void set_active(ChunkCoords coords_);

    // thread safe, atomically deinitializes Chunk, returns true if successful
    // returns false if jobs are in progress or Chunk must be saved
    // if Chunk needed to be saved, save job is sent to thread queue
    bool try_set_inactive();

    // thread safe
    BlockData get_block(ChunkIndex indices) const;
    void set_block(ChunkIndex indices, BlockData block);

    // thread safe
    void render_opaque(const Camera& camera) const;
    void render_transparent(const Camera& camera) const;

private:
    // thread safe
    // loads data from file or generates from seed if no save data
    void load_job();

    // thread safe
    void save_job();

    // thread safe
    // build meshes using chunk data
    void mesh_job();

    void calculate_translation();

    void build_opaque_vertices_and_indices(std::vector<Vertex>& vertices, std::vector<unsigned>& indices) const;
    void build_transarent_vertices_and_indices(std::vector<Vertex>& vertices, std::vector<unsigned>& indices) const;

    void append_block_face(std::vector<Vertex>& vertices, 
                           std::vector<unsigned int>& indices, 
                           const ChunkIndex& chunk_indices, const BlockData& current,
                           CubeFace face) const;

    bool should_draw_opaque_face(const ChunkIndex& index, const Direction& direction) const;

    bool should_draw_transparent_face(const ChunkIndex& index, const Direction& direction) const;

    void generate_data_from_seed();

    void load_data_from_seed();

    std::string get_chunk_filename() const;

    mutable std::shared_mutex mut;
    ChunkCoords coords;
    ChunkBlocks blocks;
    bool active;
    bool wrapping_up;
    bool can_render;
    bool dirty;
    int mesh_jobs;
    int load_jobs;
    int save_jobs;
    Mesh opaque_mesh;
    Mesh transparent_mesh;
    glm::mat4 translation;
};

struct Direction {
    std::tuple<int,int,int> vec;
    CubeFace face;
};

static const std::array<Direction, 6> directions = {{
    { {-1, 0, 0}, CubeFace::XNEG },
    { {1, 0, 0}, CubeFace::XPOS },
    { {0, -1, 0}, CubeFace::YNEG },
    { {0, 1, 0}, CubeFace::YPOS },
    { {0, 0, -1}, CubeFace::ZNEG },
    { {0, 0, 1}, CubeFace::ZPOS }
}};