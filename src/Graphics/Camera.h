#pragma once

#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

// First-person-shooter style camera
class Camera {
public:
    Camera(float aspect, float fov = glm::radians(55.f), float zNear = 0.4f, float zFar = 1000.0f)
        : aspect(aspect), fov(fov), zNear(zNear), zFar(zFar)
    {
    }

    inline glm::mat4 get_view_projection() const
    {
        return get_perspective() * get_view();
    }

    void yaw(float angle)
    {
        // use quat to transform local space to world space
        glm::vec3 world_y_axis = transform.get_orientation() * default_up();
        transform.rotate(angle, world_y_axis);
    }

    void pitch(float angle)
    {
        // clamp pitch to +-90 degrees up and down
        const float PADDING = 0.01f;
        float remaining_up = glm::angle(get_forward(), default_up());
        float remaining_down = remaining_up - glm::radians(180.f);
        if (angle >= remaining_up) {
            transform.rotate(remaining_up - PADDING, default_left());
        } else if (angle <= remaining_down) {
            transform.rotate(remaining_down + PADDING, default_left());
        } else {
            transform.rotate(angle, default_left());
        }
    }

    void move_forward(float dist)
    {
        transform.translate(get_forward() * dist);
    }

    void move_up(float dist)
    {
        transform.translate(get_up() * dist);
    }

    void move_left(float dist)
    {
        transform.translate(get_left() * dist);
    }

    const glm::vec3& get_position() const
    {
        return transform.get_pos();
    }

    /* use inverse quat to transform default vector into local space vector */
    glm::vec3 get_forward() const
    {
        return glm::conjugate(transform.get_orientation()) * default_forward();
    }
    glm::vec3 get_up() const
    {
        return glm::conjugate(transform.get_orientation()) * default_up();
    }
    glm::vec3 get_left() const
    {
        return glm::conjugate(transform.get_orientation()) * default_left();
    }

private:
    float aspect;
    float fov;
    float zNear;
    float zFar;

    Transform transform;

    static glm::vec3 default_forward()
    {
        return glm::vec3(0, 0, -1);
    }

    static glm::vec3 default_up()
    {
        return glm::vec3(0, 1, 0);
    }

    static glm::vec3 default_left()
    {
        return glm::vec3(-1, 0, 0);
    }

    /* view projection functions */
    inline glm::mat4 get_view() const
    {
        return glm::lookAt(transform.get_pos(), transform.get_pos() + get_forward(),
            get_up());
    }

    inline glm::mat4 get_perspective() const
    {
        return glm::perspective(fov, aspect, zNear, zFar);
    }
};
