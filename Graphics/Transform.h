#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
    Transform(const glm::vec3& pos = glm::vec3()
        , const glm::quat& rot = glm::quat(1.f,0.f,0.f,0.f)
        , const glm::vec3& scale = glm::vec3(1.f,1.f,1.f)) :
        position(pos), orientation(rot), m_scale(scale) {}

    inline glm::mat4 get_model() const {
        glm::mat4 position_matrix = glm::translate(position);
        glm::mat4 rotation_matrix = glm::mat4_cast(orientation);
        glm::mat4 scale_matrix = glm::scale(m_scale);

        return position_matrix * rotation_matrix * scale_matrix;
    }

    const glm::vec3& get_pos() const {
        return position;
    }
    const glm::quat& get_orientation() const {
        return orientation;
    }
    const glm::vec3& get_scale() const {
        return m_scale;
    }

    void set_pos(glm::vec3 pos) {
        position = pos;
    }
    void set_scale(glm::vec3 scale) {
        m_scale = scale;
    }

    void translate(glm::vec3 translation) {
        position += translation;
    }
    void rotate(float angle, glm::vec3 axis) {
        glm::quat rotation = glm::angleAxis(angle, axis);
        orientation = rotation * orientation;
        glm::normalize(orientation);
    }
    void scale(glm::vec3 scale) {
        m_scale += scale;
    }

protected:
private:
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 m_scale;
};

#endif // !TRANSFORM_H

