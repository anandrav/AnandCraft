#pragma once

#include "glm/glm.hpp"

class Ray {
public:
    // direction does not have to be a unit vector
    Ray(glm::vec3 origin, glm::vec3 direction) 
        : direction(glm::normalize(direction))
        , origin(origin)
        , end(origin) 
    {
    }

    void step(float distance) {
        end += distance * direction;
    }

    const glm::vec3& get_end() const {
        return end;
    }

    float get_length() const {
        return glm::length(end - origin);
    }

private:
    glm::vec3 direction;
    glm::vec3 origin;
    glm::vec3 end;
};