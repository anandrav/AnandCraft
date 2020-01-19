/*
World is a class that is responsible for the land that the player walks on.
It is not responsible for every "game object" in the simulation, only the terrain.
Terrain is procedurally generated/loaded in chunks.
*/

#pragma once

#include "Block.h"
#include "ChunkSystem.h"

class World : public Renderable {
public:
    BlockState get_block(int x, int y, int z) const;

    void modify_block(int x, int y, int z);
    
    void render_opaque() const override;

    void render_transparent() const override;

private:
    ChunkSystem chunk_system;
};