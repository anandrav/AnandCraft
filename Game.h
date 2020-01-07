#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "util.h"
#include "Graphics/Shader.h"
#include "Graphics/Transform.h"
#include "Player.h"
#include "PlayerController.h"
#include "Async/AsyncQueue.h"
#include "World/World.h"

#include "Ray.h"

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

    void handle_click(SDL_Event& event);
};

