#pragma once

#include "Block.h"
#include "Graphics/Transform.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Entity.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <memory>

class SingleBlockDemo : public Entity {
public:
    SingleBlockDemo();

    void render_opaque(const Camera& camera) const override;

    void update() override;

private:
    BlockData state;
    Shader shader;
    Mesh mesh;
    Transform transform;
    double previous;
    double lag;
    int id_index;

    void setup_mesh();

    void increment_vector(std::vector<unsigned int>& vec, int num);
};
