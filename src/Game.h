#pragma once

#include "GameObject.h"
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

    ThreadQueue& get_thread_queue() {
        return thread_queue;
    }

    SyncQueue& get_sync_queue() {
        return sync_queue;
    }

    void register_game_object(GameObject* game_object) {
        game_objects.insert(game_object);
    }

    void deregister_game_object(GameObject* game_object) {
        game_objects.erase(game_object);
    }

private:
    // TODO: move these
    Player player;
    PlayerController player_controller;
    bool is_running;

    SDL_Window* sdl_window;
    SDL_GLContext sdl_glcontext;

    std::unique_ptr<SingleBlockDemo> demo;

    ThreadQueue thread_queue;
    SyncQueue sync_queue;

    void loop();

    void update();

    void process_input();

    void render();

    std::set<GameObject*> game_objects;
};