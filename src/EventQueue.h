#pragma once

#include <functional>

// using Event_t = std::function<void (void)>;

class Event {
    virtual ~Event() = default;
}

class EventQueue {
    int placeholder;
};