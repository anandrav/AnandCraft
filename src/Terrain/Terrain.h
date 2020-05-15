/*
World is a class that is responsible for the land that the player walks on.
It is not responsible for every "game object" in the simulation, only the terrain.
Terrain is procedurally generated/loaded in chunks.
*/

#pragma once

#include "Chunk.h"
#include "../Player.h"
#include "../Graphics/Camera.h"
#include "../Entity.h"
#include "../EventListener.h"
#include "../RaycastEvent.h"

#include <unordered_map>
#include <memory>

constexpr int RENDER_DIST { 4 };

class Terrain : public Entity {
public:
    Terrain(Player* player);

    ~Terrain();

    void update() override;

    void render_opaque(const Camera& camera) const override;
    void render_transparent(const Camera& camera) const override;

private:
    struct ChunkCoordsHash {
        std::size_t operator() (const ChunkCoords& coords) const {
            std::size_t h1 = std::hash<int>()(coords.x);
            std::size_t h2 = std::hash<int>()(coords.y);
            std::size_t h3 = std::hash<int>()(coords.z);
            return h1 ^ h2 ^ h3;
        }
    };

    bool handle_raycast_event(std::shared_ptr<RaycastEvent> event);

    Player* player;
    std::unordered_map<ChunkCoords, std::shared_ptr<Chunk>, ChunkCoordsHash> chunks;
    EventListener<RaycastEvent> raycast_listener;
};