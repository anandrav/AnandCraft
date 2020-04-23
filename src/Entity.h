/*
 * Entity is anything in the Game that can be updated and/or rendered.
 * Prefer composition over inheritance for adding functionality.
 * Entities do not have an explicit scene hierarchy, 
 * but their components (transforms) may.
 */

#pragma once

#include "Graphics/Camera.h"

using ID_t = uint64_t;

class Entity {
public:
    const ID_t ID;
    const char* const name;

    // register with Game, initialize ID, optionally give name
    Entity(const char* const name = nullptr);

    // deregister from Game
    virtual ~Entity();

    // fat interface
    virtual void update() {}
    virtual void render_opaque(const Camera&) const {}
    virtual void render_transparent(const Camera&) const {}
};