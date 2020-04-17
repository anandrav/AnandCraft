/*
World is a class that is responsible for the land that the player walks on.
It is not responsible for every "game object" in the simulation, only the terrain.
Terrain is procedurally generated/loaded in chunks.
*/

#pragma once

#include "Chunk.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Renderable.h"

#include <unordered_map>

class Terrain : public Renderable {
public:
    // BlockData get_block(int x, int y, int z) const;

    // void modify_block(int x, int y, int z);
    
    void render_opaque(const Camera& camera) const override;

    void render_transparent(const Camera& camera) const override;

private:
    std::unordered_map<ChunkCoords, ChunkPtr> chunks;
};