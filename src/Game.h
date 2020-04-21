/*
 * Game represents the global state of the application.
 */
#pragma once

#include "Entity.h"
#include "Event.h"
#include "Jobs/ThreadQueue.h"
#include "Jobs/SyncQueue.h"

#include "PlayerController.h"
#include "Player.h"
#include "BlockDemo.h"

#include <unordered_map>
#include <set>
#include <memory>
#include <typeindex>
#include <cassert>

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

    template<typename EventType>
    void register_listener(std::pair<ID_t, EventHandler_t> ID_handler_pair);

    template<typename EventType>
    void deregister_listener(std::pair<ID_t, EventHandler_t> ID_handler_pair);

    template<typename EventType>
    void dispatch_event(std::shared_ptr<EventType> event);

private:
    bool is_running;

    SDL_Window* sdl_window;
    SDL_GLContext sdl_glcontext;

    struct HandlersHash {
        std::size_t operator() (const std::pair<std::type_index,ID_t>& pair) const {
            std::size_t h1 = std::hash<std::type_index>()(pair.first);
            std::size_t h2 = std::hash<ID_t>()(pair.second);
            return h1 ^ h2;
        }
    };
    std::unordered_multimap<std::type_index, std::pair<ID_t, EventHandler_t>> handlers;

    ThreadQueue thread_queue;
    SyncQueue sync_queue;

    std::set<Entity*> entities;

    // TODO: move these to another class?
    std::unique_ptr<Terrain> terrain;
    std::unique_ptr<SingleBlockDemo> demo;
    std::unique_ptr<Player> player;
    std::unique_ptr<PlayerController> player_controller;

    void update();

    void process_input();

    void render();
};

template<typename EventType>
void Game::register_listener(std::pair<ID_t, EventHandler_t> ID_handler_pair) {
    handlers.insert({std::type_index(typeid(EventType)), ID_handler_pair});
}

template<typename EventType>
void Game::deregister_listener(std::pair<ID_t, EventHandler_t> ID_handler_pair) {
    auto [begin, end] = handlers.equal_range(std::type_index(typeid(EventType)));
    auto& it = begin;
    for (; it != end; ++it) {
        // only compare ID for equality
        if (it->second.first == ID_handler_pair.first) { 
            handlers.erase(it);
            return;
        }
    }
    throw std::runtime_error("EventHandler could not be deregistered");
}

template<typename EventType>
void Game::dispatch_event(std::shared_ptr<EventType> event) {
    auto [begin, end] = handlers.equal_range(std::type_index(typeid(EventType)));
    auto& it = begin;
    for(; it != end; ++it) {
        if ((it->second)(event))
            break;
    }
}