#pragma once

#include "Event.h"
#include "Game.h"
#include "EventDispatcher.h"

template<typename EventType>
class EventListener {
public:
    EventListener(ID_t ID, std::function<bool (std::shared_ptr<EventType>)> handler) 
        : ID(ID)
    {
        g_game->get_event_dispatcher().register_listener<EventType>(ID, 
            [handler](std::shared_ptr<EventBase> e) -> bool {
                auto downcasted = std::static_pointer_cast<EventType>(e);
                return handler(downcasted);
            }
        );
    }

    ~EventListener() {
        g_game->get_event_dispatcher().deregister_listener<EventType>(ID);
    }

private:
    ID_t ID;
};