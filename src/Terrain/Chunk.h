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

#include <shared_mutex>
#include <array>

struct DIRECTION;

class Chunk : public Entity {
public:
    Chunk(ChunkCoords coords);

    Chunk(Chunk&) = delete;
    Chunk& operator=(Chunk&) = delete;

    void render_opaque(const Camera& camera) const override;
    void render_transparent(const Camera& camera) const override;

    void build_meshes();

private:
    void update_opaque_mesh();
    void update_transparent_mesh();

    void append_block_face(std::vector<Vertex>& vertices, 
                           std::vector<unsigned int>& indices, 
                           int x, int y, int z, const BlockData& current,
                           CubeFace face) const;

    bool is_transparent_at(int x, int y, int z) const;

    bool is_same_material_at(BlockData& current, int x, int y, int z) const;

    mutable std::shared_mutex mut;
    ChunkCoords coords;
    ChunkBlocks blocks;
    Mesh opaque_mesh;
    Mesh transparent_mesh;
    glm::mat4 translation;
};

struct DIRECTION {
    int x,y,z;
    CubeFace face;
};

static const std::array<DIRECTION, 6> directions = {{
    { -1, 0, 0, CubeFace::XNEG },
    { 1, 0, 0, CubeFace::XPOS },
    { 0, -1, 0, CubeFace::YNEG },
    { 0, 1, 0, CubeFace::YPOS },
    { 0, 0, -1, CubeFace::ZNEG },
    { 0, 0, 1, CubeFace::ZPOS }
}};