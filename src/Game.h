/*
 * Game represents the global state of the application.
 */
#pragma once

#include "Entity.h"
#include "EventDispatcher.h"
#include "Jobs/ThreadQueue.h"
#include "Jobs/SyncQueue.h"
#include "Player.h"
#include "PlayerController.h"

#include <set>

#include <SDL2/SDL.h>

struct SDL_Window;
class Game;

// global game state
extern Game* g_game;

class Game {
public:
    Game();

    ~Game();

    void initialize_scene();

    void loop();

    ThreadQueue& get_thread_queue() {
        return thread_queue;
    }

    SyncQueue& get_sync_queue() {
        return sync_queue;
    }

    EventDispatcher& get_event_dispatcher() {
        return event_dispatcher;
    }

    Player& get_player() {
        return player;
    }

    void register_entity(Entity* entity);

    void deregister_entity(Entity* entity);

private:
    bool is_running;

    SDL_Window* sdl_window;
    SDL_GLContext sdl_glcontext;

    std::set<Entity*> entities;

    EventDispatcher event_dispatcher;

    Player player;
    PlayerController controller;

    SyncQueue sync_queue;
    // destroy this first. threads must be joined so they don't access Game during destruction
    ThreadQueue thread_queue;

    // TODO: reorganize so you don't have to use this hack.
    void* scene;

    void update();

    void process_input();

    void render();
};