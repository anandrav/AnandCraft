#pragma once

#include "Player.h"
#include "PlayerController.h"
#include "Ray.h"
#include "BlockDemo.h"

// forward declarations
struct SDL_Window;

class Game {
public:
    Game(SDL_Window* window);

    void run_loop();

private:
    SingleBlockDemo demo;
    unsigned int texture; // FIXME move this to the Terrain class or the World folder or something.

    Player player;
    PlayerController player_controller;

    bool is_running;

    SDL_Window* window;

    void process_input();

    void update();

    void render();
};

