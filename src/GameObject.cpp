#include "GameObject.h"

GameObject::GameObject() {
    g_game->register_game_object(this);
}

GameObject::~GameObject() {
    g_game->deregister_game_object(this);
}