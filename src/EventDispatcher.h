#pragma once

#include "Event.h"
#include "ID.h"

#include <map>
#include <memory>
#include <typeindex>
#include <string>
#include <stdexcept>

class EventDispatcher {
public:
    template<typename EventType>
    void register_listener(ID_t ID, EventHandler_t handler);

    template<typename EventType>
    void deregister_listener(ID_t ID);

    template<typename EventType>
    void dispatch_event(std::shared_ptr<EventType> event);
    
private:
    std::map<std::pair<std::type_index, ID_t>, EventHandler_t> handlers;
};

template<typename EventType>
void EventDispatcher::register_listener(ID_t ID, EventHandler_t handler) {
    bool success = handlers.insert({ { std::type_index(typeid(EventType)), ID }, handler }).second;
    if (!success)
        throw std::runtime_error("Listener of this type already registered for Entity ID: " + std::to_string(ID));
}

template<typename EventType>
void EventDispatcher::deregister_listener(ID_t ID) {
    bool success = handlers.erase({ std::type_index(typeid(EventType)), ID });
    if (!success)
        throw std::runtime_error("EventHandler could not be deregistered");
}

template<typename EventType>
void EventDispatcher::dispatch_event(std::shared_ptr<EventType> event) {
    auto it = handlers.lower_bound({ std::type_index(typeid(EventType)), 0 });
    for (; it != handlers.end() && it->first.first == std::type_index(typeid(EventType)); ++it) {
        if (it->second(event))
            return;
    }
}