/*
World is a class that is responsible for the land that the player walks on.
It is not responsible for every "game object" in the simulation, only the terrain.
Terrain is procedurally generated/loaded in chunks.
*/

#pragma once

#include "Block.h"
#include "ChunkSystem.h"

class World {
public:

    BlockState get_block(int x, int y, int z);

    void modify_block(int x, int y, int z);

private:
    ChunkSystem chunk_system;
};

