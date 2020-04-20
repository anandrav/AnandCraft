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