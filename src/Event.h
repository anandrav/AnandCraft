#pragma once

#include <functional>
#include <memory>

// inherit EventType from this
struct EventBase {};

// return true if event was "consumed"
using EventHandler_t = std::function<bool (std::shared_ptr<EventBase>)>;