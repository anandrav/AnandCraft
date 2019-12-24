#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "util.h"
#include "Graphics/Shader.h"
#include "Graphics/Transform.h"
#include "Graphics/Camera.h"
#include "Async/AsyncQueue.h"
#include "World/World.h"

#include "Ray.h"

class Game {
public:
    // singleton pattern
    static Game& instance() {
        static Game singleton;
        return singleton;
    }

    // disallow copying and moving singleton
    Game(Game&) = delete;
    Game& operator=(Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    void run_loop();

    static const unsigned int WIDTH = 1280;
    static const unsigned int HEIGHT = 720;

private:
    // disallow construction or accidental deletion of singleton
    Game();
    ~Game();

    void init();

    Camera camera;
    glm::vec3 camera_vel;
    glm::vec3 camera_rot;
    unsigned int texture;
    World* world;

    bool is_running;
    SDL_Window* main_window;
    SDL_GLContext main_context;

    void process_input();

    void update();

    void render();

    void set_OpenGL_attributes();

    void handle_click(SDL_Event& event);
};

