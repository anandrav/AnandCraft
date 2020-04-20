/*
World is a class that is responsible for the land that the player walks on.
It is not responsible for every "game object" in the simulation, only the terrain.
Terrain is procedurally generated/loaded in chunks.
*/

#pragma once

#include "Chunk.h"
#include "../Graphics/Camera.h"
#include "../Entity.h"

#include <unordered_map>
#include <memory>

class Terrain {
public:
    Terrain();

private:
    struct ChunkCoordsHash {
        std::size_t operator() (const ChunkCoords& coords) const {
            std::size_t h1 = std::hash<int>()(coords.x);
            std::size_t h2 = std::hash<int>()(coords.y);
            std::size_t h3 = std::hash<int>()(coords.z);
            return h1 ^ h2 ^ h3;
        }
    };

    std::unordered_map<ChunkCoords, std::shared_ptr<Chunk>, ChunkCoordsHash> chunks;
};