#pragma once

#include "Player.h"
#include "PlayerController.h"
#include "Ray.h"
#include "BlockDemo.h"

#include <memory>

// forward declarations
struct SDL_Window;
class Game;

// global game state
extern Game* g_game;

class Game {
public:
    Game();

    ~Game();

private:
    // TODO: move these
    Player player;
    PlayerController player_controller;
    bool is_running;

    SDL_Window* sdl_window;
    SDL_GLContext sdl_glcontext;

    std::unique_ptr<SingleBlockDemo> demo;

    void run_loop();

    void process_input();

    void update();

    void render();

    // std::vector<weak_ptr<GameObject>>; // for_each(, , [](){update});
    // std::vector<weak_ptr<Renderable>>; // for_each(, , [](){render});
};

