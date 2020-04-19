#pragma once

#include "Player.h"
#include "SDL2/SDL_events.h"

class PlayerController {
public:
    PlayerController(Player* player = nullptr);

    void process_event(SDL_Event &e);

private:
    Player* player;

    void handle_click(SDL_Event &e);
};