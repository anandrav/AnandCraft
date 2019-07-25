#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
public:
    Camera(float aspect, const glm::vec3& pos = glm::vec3(0.f,0.f,-2.f), float fov=70.0f, float zNear=0.01f, float zFar=1000.0f) {
        perspective = glm::perspective(fov, aspect, zNear, zFar);
        position = pos;
        forward = glm::vec3(0, 0, 1);
        up = glm::vec3(0, 1, 0);
    }

    inline glm::mat4 GetViewProjection() const {
        return perspective * glm::lookAt(position, position + forward, up);
    }

    // methods for repositioning and rotating camera
    void reset_rotation() {
        forward = glm::vec3(0, 0, 1);
        up = glm::vec3(0, 1, 0);
    }

    void rotate_x(float degrees) {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(degrees), glm::vec3(1.0, 0.0, 0.0));
        forward = transform * glm::vec4(forward, 1.0f);
        up = transform * glm::vec4(up, 1.0f);
    }

    void rotate_y(float degrees) {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(degrees), glm::vec3(0.0, 1.0, 0.0));
        forward = transform * glm::vec4(forward, 1.0f);
        up = transform * glm::vec4(up, 1.0f);
    }

    void rotate_z(float degrees) {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(degrees), glm::vec3(0.0, 0.0, 1.0));
        forward = transform * glm::vec4(forward, 1.0f);
        up = transform * glm::vec4(up, 1.0f);
    }

    void set_position(const glm::vec3& pos) {
        position = pos;
    }

    void translate(const glm::vec3& translation) {
        position = position + translation;
    }

protected:
private:
    glm::mat4 perspective;
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
};
