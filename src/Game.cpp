#include "Game.h"

#include "util.h"
#include "Jobs/SyncQueue.h"
#include "globals.h"
#include "SceneCoordinator.h"
#include "Scene.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

const int TICKRATE = 20;
const int MS_PER_UPDATE = 1000 / TICKRATE;

void set_OpenGL_attributes();
void check_SDL_error(int line);

Game::Game()
    : is_running(true)
{
    // Set up SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Failed to init SDL" << endl;
    }
    SDL_SetRelativeMouseMode(SDL_TRUE);

    set_OpenGL_attributes();

    // create our window centered at WIDTH x HEIGHT resolution
    sdl_window = SDL_CreateWindow(
        "AnandCraft",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_OPENGL
    );
    if (!sdl_window) {
        cout << "Unable to create window" << endl;
        check_SDL_error(__LINE__);
    }
    // Set up OpenGL
    sdl_glcontext = SDL_GL_CreateContext(sdl_window);
    if (!sdl_glcontext)
        check_SDL_error(__LINE__);

    // vertical sync
    SDL_GL_SetSwapInterval(1);

    // init GLEW
    glewExperimental = GL_TRUE;
    glewInit();
}

Game::~Game() {
    // Clean up SDL and OpenGL
    SDL_GL_DeleteContext(sdl_glcontext);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void Game::run()
{
    controller.attach(g_player);

    double previous = SDL_GetTicks();
    double lag = 0.0;
    while (is_running) {
        double current = SDL_GetTicks();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        while (lag >= MS_PER_UPDATE) {
            update();
            lag -= MS_PER_UPDATE;
        }

        // const double interpolate = lag / MS_PER_UPDATE;
        render();
    }
}

void Game::update()
{
    process_input();
    // update all game objects
    g_scene->update();

    sync_queue.process_all();
}

void Game::process_input()
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            is_running = false;
            g_scene_coordinator->destroy();
        } else {
            controller.process_event(e);
        }
    }
}

void Game::render()
{
    // clear
    glDepthMask(GL_TRUE);
    glClearColor(.60f, .70f, .95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render all game objects
    g_scene->render();

    // render UI/HUD

    // update display
    SDL_GL_SwapWindow(sdl_window);
}

void set_OpenGL_attributes()
{
    // deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // version 3.2
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void check_SDL_error(int line = -1) {
    string error = SDL_GetError();
    if (error != "") {
        cout << "SDL Error : " << error << endl;
        if (line != -1) {
            cout << "\nLine : " << line << endl;
        }
        SDL_ClearError();
    }
}