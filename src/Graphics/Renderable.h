/*
Abstract interface for any object in the scene that can be rendered.
All objects' opaque components should be rendered first in one pass.
Then, all objects' transparent components should be rendered in another pass.
*/

#pragma once

#include "Camera.h"

class Renderable {
public:
    virtual void render_opaque(const Camera& camera) const = 0;
    virtual void render_transparent(const Camera& camera) const = 0;
};