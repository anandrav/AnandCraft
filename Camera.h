#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// First-person camera
class Camera {
public:
    Camera(float aspect, float fov = glm::radians(70.f), float zNear = 0.01f,
        float zFar = 1000.0f) :
        aspect(aspect), fov(fov), zNear(zNear), zFar(zFar) {
    }

    inline glm::mat4 get_view_projection() {
        return get_perspective() * get_view();
    }

    void yaw(float angle) {
        transform.rotate(angle, transform.get_orientation() * default_up());
    }

    void pitch(float angle) {
        transform.rotate(angle, transform.get_orientation() * get_left());
    }

    void move_forward(float dist) {
        transform.translate(get_forward() * dist);
    }

    void move_up(float dist) {
        transform.translate(get_up() * dist);
    }

    void move_left(float dist) {
        transform.translate(get_left() * dist);
    }

private:
    float aspect;
    float fov;
    float zNear;
    float zFar;

    Transform transform;

    static glm::vec3 default_forward() {
        return glm::vec3(0, 0, -1);
    }

    static glm::vec3 default_up() {
        return glm::vec3(0, 1, 0);
    }

    static glm::vec3 default_left() {
        return glm::vec3(-1, 0, 0);
    }

    glm::vec3 get_forward() {
        return glm::conjugate(transform.get_orientation()) * default_forward();
    }

    glm::vec3 get_up() {
        return glm::conjugate(transform.get_orientation()) * default_up();
    }

    glm::vec3 get_left() {
        return glm::conjugate(transform.get_orientation()) * default_left();
    }

    glm::mat4 get_view() {
        return glm::lookAt(transform.get_pos(), transform.get_pos() + get_forward(), get_up());
    }

    glm::mat4 get_perspective() {
        return glm::perspective(fov, aspect, zNear, zFar);
    }
};
