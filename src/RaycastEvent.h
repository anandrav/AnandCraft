#pragma once

#include "Event.h"
#include "Physics/Ray.h"
#include "glm/glm.hpp"


class RaycastEvent : public EventBase {
public:
    RaycastEvent(const Ray& ray, bool left_click);

    Ray get_ray() {
        return ray;
    }

    bool is_left_click() {
        return left_click;
    }

    glm::vec3& get_closest() {
        return closest;
    }

    void set_callback(const std::function<void (void)>& callback_in) {
        callback = callback_in;
    }

    void on_finished_dispatch() override {
        if (callback)
            callback();
    }

private:
    Ray ray;
    bool left_click;
    glm::vec3 closest;
    std::function<void (void)> callback;
};