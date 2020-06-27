/*
 * Game represents the global state of the application.
 */
#pragma once

#include "Jobs/ThreadQueue.h"
#include "Jobs/SyncQueue.h"
#include "PlayerController.h"

#include <set>
#include <GL/glew.h>
#include <SDL2/SDL.h>

class Entity;

class Game {
public:
    Game();

    ~Game();

    void run();

    ThreadQueue& get_thread_queue() {
        return thread_queue;
    }

    SyncQueue& get_sync_queue() {
        return sync_queue;
    }

    void register_entity(Entity* entity);

    void deregister_entity(Entity* entity);

private:
    bool is_running;

    SDL_Window* sdl_window;
    SDL_GLContext sdl_glcontext;

    PlayerController controller;

    SyncQueue sync_queue;
    // destroy this first. threads must be joined so they don't access Game during destruction
    ThreadQueue thread_queue;

    void update();

    void process_input();

    void render();
};