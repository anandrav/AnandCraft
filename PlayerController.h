#pragma once

#include "Player.h"
#include "World/World.h"
#include "SDL2/SDL_events.h"

class PlayerController {
public:
    PlayerController(Player* player_in, World* world_in);

    void process_event(SDL_Event &e);

private:
    Player* player;
    World* world;

    void handle_click(SDL_Event &e);
};