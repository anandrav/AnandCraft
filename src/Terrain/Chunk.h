/*
 * Chunk.h
 * 
 * Chunk of blocks.
 */

#pragma once

#include "ChunkCoords.h"
#include "ChunkData.h"
#include "WorldConfig.h"
#include "../GameObject.h"
#include "../Graphics/Mesh.h"

#include <shared_mutex>

class Chunk : public GameObject {
public:
    Chunk(ChunkCoords coords);

    Chunk(Chunk&) = delete;
    Chunk& operator=(Chunk&) = delete;

    void render_opaque(const Camera& camera) const override;
    void render_transparent(const Camera& camera) const override;

    void update_meshes();

private:
    void update_opaque_mesh();
    void update_transparent_mesh();

    void append_block_face(std::vector<Vertex>& vertices, 
                           std::vector<unsigned int>& indices, 
                           int x, int y, int z, const BlockData& block, 
                           CubeFace face) const;

    bool is_transparent_at(int x, int y, int z) const;

    std::shared_mutex mutex;
    ChunkCoords coords;
    ChunkBlocks blocks;
    Mesh opaque_mesh;
    Mesh transparent_mesh;
    glm::mat4 translation;
};