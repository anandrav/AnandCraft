#pragma once

#include "EventDispatcher.h"

#include <memory>
#include <set>

class Entity;

class Scene {
public:
    void register_entity(Entity* entity);

    void deregister_entity(Entity* entity);

    void update();
    
    void render() const;

private:
    std::set<Entity*> entities;
};