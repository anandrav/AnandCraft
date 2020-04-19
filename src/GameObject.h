#pragma once

#include "Graphics/Camera.h"

class GameObject {
public:
    // register with Game
    GameObject();

    // deregister from Game
    virtual ~GameObject();

    // fat interface
    virtual void update() {}
    virtual void render_opaque(const Camera&) const {}
    virtual void render_transparent(const Camera&) const {}
};