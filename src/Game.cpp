#include "Game.h"

#include "util.h"
#include "Async/SyncQueue.h"
#include "globals.h"
#include "Terrain/TerrainTexture.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

using namespace std;

const int TICKRATE = 20;
const int MS_PER_UPDATE = 1000 / TICKRATE;

Game::Game(SDL_Window* window_in)
    : window(window_in)
    , player_controller(&player)
    , is_running(true)
{
}

void Game::run_loop()
{
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

        const double interpolate = lag / MS_PER_UPDATE;
        render();
    }
}

void Game::process_input()
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            is_running = false;
        } else {
            player_controller.process_event(e);
        }
    }
}

void Game::update()
{
    process_input();

    player.update();
    // world.update(player.get_position());
    demo.update();

    // fixme put this in a while loop
    // while (!deterministic_job_queue.empty()) or something
    // SyncQueue::get_instance().process_all_tasks();
}

void Game::render()
{
    // clear
    glDepthMask(GL_TRUE);
    glClearColor(.60f, .70f, .95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render world
    // glBindTexture(GL_TEXTURE_2D, texture);
    demo.render(player.get_camera());
    // world.render_opaque(player.get_camera());
    // world.render_transparent(player.get_camera());

    // render UI/HUD

    // update display
    SDL_GL_SwapWindow(window);
}