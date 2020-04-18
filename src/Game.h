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
    // char if_I_delete_this_bug_but_why;

private:
    SDL_Window* window;

    Player player;
    PlayerController player_controller;
    bool is_running;

    SingleBlockDemo demo;

    void process_input();

    void update();

    void render();
};

