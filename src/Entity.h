/*
 * Entity is anything in the Game that can be updated and/or rendered.
 * Prefer composition over inheritance for adding functionality.
 * Entities do not have an explicit scene hierarchy, 
 * but their components (transforms) may.
 */

#pragma once

#include "Graphics/Camera.h"

class Entity {
public:
    // register with Game
    Entity();

    // deregister from Game
    virtual ~Entity();

    // fat interface
    virtual void update() {}
    virtual void render_opaque(const Camera&) const {}
    virtual void render_transparent(const Camera&) const {}
};