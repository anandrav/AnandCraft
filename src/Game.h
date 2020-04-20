#pragma once

#include "Entity.h"
#include "Player.h"
#include "PlayerController.h"
#include "BlockDemo.h"
#include "Jobs/ThreadQueue.h"
#include "Jobs/SyncQueue.h"

#include <set>
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

    ThreadQueue thread_queue;
    SyncQueue sync_queue;

    std::set<Entity*> entities;

    // TODO: move these
    std::unique_ptr<Terrain> terrain;
    std::unique_ptr<SingleBlockDemo> demo;
    std::unique_ptr<Player> player;
    std::unique_ptr<PlayerController> player_controller;

    void update();

    void process_input();

    void render();
};