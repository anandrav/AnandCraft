#pragma once
#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "util.h"
#include "Transform.h"
#include "Camera.h"
#include "Model.h"

class Game {
public:
    Game();

    ~Game();

    void run_loop();

    static const unsigned int WIDTH = 800;
    static const unsigned int HEIGHT = 600;

private:
    void init();
    Camera camera;
    glm::vec3 camera_vel;
    glm::vec3 camera_rot_vel;
    Model model;

    const int MINECRAFT_TICKRATE = 20;
    const int MS_PER_UPDATE = 1000 / MINECRAFT_TICKRATE;

    bool is_running;
    SDL_Window* main_window;
    SDL_GLContext main_context;

    void process_input();

    void update();

    void render();

    void set_OpenGL_attributes();
};

