/*
 * Chunk.h
 * 
 * Chunk of blocks.
 */

#pragma once

#include "ChunkCoords.h"
#include "ChunkData.h"
#include "WorldConfig.h"
#include "../Graphics/Renderable.h"
#include "../Graphics/Mesh.h"

#include <memory>
#include <shared_mutex>

struct Chunk;
using ChunkPtr = std::shared_ptr<Chunk>;

struct Chunk : public Renderable {
    Chunk(ChunkCoords coords_)
        : coords(coords_)
    {
    }

    void render_opaque(const Camera& camera) const override;
    void render_transparent(const Camera& camera) const override;

    std::shared_mutex mutex;
    ChunkCoords coords;
    ChunkData data;
    Mesh opaque_mesh;
    Mesh transparent_mesh;
};