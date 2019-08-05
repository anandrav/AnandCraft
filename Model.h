#pragma once

#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Transform.h"

// composition of mesh, shader, and transform
class Model {
public:
    Model() = default;

    Model(Mesh mesh, Shader shader, Transform transform = Transform()) :
        mesh(mesh), shader(shader), transform(transform) {
    }

    virtual void render(Camera& camera) {
        shader.bind();
        glm::mat4 clip_transform = camera.get_view_projection() * transform.get_model();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, &clip_transform[0][0]);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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