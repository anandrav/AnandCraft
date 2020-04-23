#pragma once

#include <functional>
#include <memory>

// your EventType should inherit from this
class EventBase {
public:
    virtual ~EventBase() = default;

    virtual void on_finished_dispatch() {}
};

using EventHandler_t = std::function<bool (std::shared_ptr<EventBase>)>;