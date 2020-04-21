#include "Entity.h"

#include "Game.h"

#include <iostream>

using namespace std;

ID_t max_ids { 0 };

Entity::Entity()
    : ID(max_ids++)
{
    g_game->register_entity(this);
    cout << "entity registered" << endl;
}

Entity::~Entity() 
{
    g_game->deregister_entity(this);
    cout << "entity gone" << endl;
}