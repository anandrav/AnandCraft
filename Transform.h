#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform {
public:
    Transform(const glm::vec3& pos = glm::vec3(),
        const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3()) :
        m_pos(pos), m_rot(rot), m_scale(scale) {}

    inline glm::mat4 GetModel() const {
        glm::mat4 posMatrix = glm::translate(m_pos);
        glm::mat4 rotXMatrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
        glm::mat4 rotYMatrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
        glm::mat4 rotZMatrix = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));
        glm::mat4 scaleMatrix = glm::translate(m_scale);

        glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

        return posMatrix * rotMatrix * scaleMatrix;
    }

    glm::vec3& getPos() {
        return m_pos;
    }
    glm::vec3& getRot() {
        return m_rot;
    }
    glm::vec3& getScale() {
        return m_scale;
    }
protected:
private:
    glm::vec3 m_pos;
    glm::vec3 m_rot;
    glm::vec3 m_scale;
};

#endif // !TRANSFORM_H

