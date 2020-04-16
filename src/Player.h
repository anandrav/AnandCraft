#pragma once

#include "Graphics/Camera.h"

class Player {
public:
    Player();

    void update();

    // Getters

    const glm::vec3& get_position() const
    {
        return camera.get_position();
    }

    const glm::vec3& get_velocity() const
    {
        return velocity;
    }

    glm::vec3 get_forward() const
    {
        return camera.get_forward();
    }

    const Camera& get_camera() const
    {
        return camera;
    }

    // Setters

    void set_velocity_x(float val)
    {
        velocity.x = val;
    }

    void set_velocity_y(float val)
    {
        velocity.y = val;
    }

    void set_velocity_z(float val)
    {
        velocity.z = val;
    }

    void pitch(float angle)
    {
        camera.pitch(angle);
    }

    void yaw(float angle)
    {
        camera.yaw(angle);
    }

private:
    glm::vec3 velocity;
    glm::vec2 rotation;
    Camera camera;
};

