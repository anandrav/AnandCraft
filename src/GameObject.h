#pragma once

#include "Game.h"

class GameObject {
public:
    // register with Game
    GameObject();

    // deregister from Game
    ~GameObject();

    virtual void update() = 0;
};