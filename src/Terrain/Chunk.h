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

struct Direction;

class Chunk : public Entity {
public:
    Chunk(ChunkCoords coords);

    Chunk(Chunk&) = delete;
    Chunk& operator=(Chunk&) = delete;

    void render_opaque(const Camera& camera) const override;
    void render_transparent(const Camera& camera) const override;

    // thread safe
    // loads data from file or generates from seed if no save data
    void load_data();

    // thread safe
    void save_data();

    // thread safe
    // build meshes using chunk data
    void build_meshes();

    // thread safe
    BlockData get_block(ChunkIndex indices) const;
    void set_block(ChunkIndex indices, BlockData block);

private:
    void update_opaque_mesh();
    void update_transparent_mesh();

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
    bool dirty;
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