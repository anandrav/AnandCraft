#pragma once

#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Transform.h"

/*TODO*/

// high level abstraction for any physical object in the scene
class Model {
public:
    Model() = default;

    Model(Mesh mesh, Shader shader) : mesh(mesh), shader(shader) {}

    void render(Camera& camera) {
        shader.use();
        glm::mat4 model = camera.GetViewProjection() * transform.GetModel();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, &model[0][0]);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        mesh.draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    Transform& get_transform() {
        return transform;
    }

private:
    Mesh mesh;
    Shader shader;
    Transform transform;
};