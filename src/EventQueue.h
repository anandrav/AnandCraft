#pragma once

#include <functional>

using Event_t = std::function<void (void)>;

class EventQueue {
    int placeholder;
};