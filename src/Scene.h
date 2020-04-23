#pragma once

#include "Terrain/Terrain.h"
#include "BlockDemo.h"
#include "Player.h"

#include <memory>

class Scene {
public:
    Scene();
private:
    Terrain terrain;
    SingleBlockDemo demo;
};