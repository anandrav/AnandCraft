#include "Entity.h"

#include "Game.h"

#include <iostream>

using namespace std;

Entity::Entity() {
    g_game->register_entity(this);
    cout << "entity registered" << endl;
}

Entity::~Entity() {
    g_game->deregister_entity(this);
    cout << "entity gone" << endl;
}