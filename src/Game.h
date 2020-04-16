#pragma once

#include <iostream>

#include "Player.h"
#include "PlayerController.h"
#include "World/World.h"

#include "Ray.h"

// forward declarations
struct SDL_Window;

class Game {
public:
    Game(SDL_Window* window);

    void run_loop();

private:
    unsigned int texture;

    World world;
    Player player;
    PlayerController player_controller;

    bool is_running;

    SDL_Window* window;

    void process_input();

    void update();

    void render();
};

