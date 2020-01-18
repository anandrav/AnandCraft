#pragma once

#include "Player.h"
#include "World/World.h"
#include "SDL2/SDL_events.h"

class PlayerController {
public:
    PlayerController(Player* player_in, OldWorld* world_in);

    void process_event(SDL_Event &e);

private:
    Player* player;
    OldWorld* world;

    void handle_click(SDL_Event &e);
};