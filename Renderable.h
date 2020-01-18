#pragma once

class Renderable {
public:
    virtual void render_opaque() const = 0;
    virtual void render_transparent() const = 0;
};