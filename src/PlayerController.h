#pragma once

#include "SDL2/SDL_events.h"

class Player;

class PlayerController {
public:
    PlayerController(Player* player = nullptr);

    void process_event(SDL_Event &e);

    void attach(Player* player_) {
        player = player_;
    }

private:
    Player* player;

    void handle_click(SDL_Event &e);
};