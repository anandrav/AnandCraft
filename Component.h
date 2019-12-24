#pragma once

#include "Entity.h"

#include <vector>
#include <map>

struct GraphicsComponent {
    int stuff;
};

class GraphicsComponentManager {
public:
    void render();

private:
    std::vector<GraphicsComponent> components;
    std::map<Entity, size_t> indices;
};
