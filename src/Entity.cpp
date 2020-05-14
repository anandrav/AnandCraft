#include "Entity.h"

#include "Game.h"

#include <iostream>

using namespace std;

ID_t max_ids { 0 };

Entity::Entity(const char* const name)
    : ID(max_ids++)
    , name(name)
{
    g_game->register_entity(this);

    cout << "Entity created";
    if (name)
        cout << '\t' << name;
    cout << "\tID: " << ID << '\n';
}

Entity::~Entity()
{
    g_game->deregister_entity(this);
    
    cout << "Entity destroyed";
    if (name)
        cout << '\t' << name;
    cout << "\tID: " << ID << '\n';
}