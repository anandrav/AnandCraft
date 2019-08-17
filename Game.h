#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Graphics/Shader.h"
#include "Graphics/Mesh.h"
#include "util.h"
#include "Graphics/Transform.h"
#include "Graphics/Camera.h"
#include "Graphics/Model.h"
#include "Block.h"
#include "stb_image.h"
#include "BlockDemo.h"
#include "World/Grid.h"
#include "Ray.h"
#include "Async/AsyncQueue.h"
#include <functional>

class Game {
public:
    Game();

    ~Game();

    void run_loop();

    static const unsigned int WIDTH = 1280;
    static const unsigned int HEIGHT = 720;

private:
    void init();
    Camera camera;
    glm::vec3 camera_vel;
    glm::vec3 camera_rot;
    std::function<void(void)> func;
    Grid* grid;
    unsigned int texture;

    const int MINECRAFT_TICKRATE = 20;
    const int MS_PER_UPDATE = 1000 / MINECRAFT_TICKRATE;

    bool is_running;
    SDL_Window* main_window;
    SDL_GLContext main_context;

    void process_input();

    void update();

    void render();

    void set_OpenGL_attributes();

    void handle_click(SDL_Event& event);
};

