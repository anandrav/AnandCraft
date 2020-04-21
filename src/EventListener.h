#pragma once

#include "Event.h"
#include "Game.h"

// RAII wrapper for registering a listener
template<typename EventType>
class EventListener {
public:
    EventListener(std::pair<ID_t, EventHandler_t> pair) {
        g_game->register_listener<EventType>(pair);
    }

    ~EventListener() {
        g_game->deregister_listener<EventType>(pair);
    }
};