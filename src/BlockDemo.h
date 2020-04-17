#pragma once

#include "Block.h"
#include "Graphics/Transform.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include <iostream>
#include <memory>

class SingleBlockDemo {
public:
    Transform transform;

    SingleBlockDemo(BlockData state = BlockData(BlockID::GRASS)) : state(state) {
        shader = Shader("res/basic_vert.glsl", "res/basic_frag.glsl");
        setup_mesh();
        transform.set_pos({0,0,-2});
    }

    void render(const Camera& camera) const {
        glUseProgram(shader.ID);
        glm::mat4 clip_transform = camera.get_view_projection() * transform.get_model();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, &clip_transform[0][0]);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);

        mesh.draw();
    }

    void set_block(BlockData state) {
        this->state = state;

        setup_mesh();
    }

private:
    BlockData state;
    Shader shader;
    Mesh mesh;

    void setup_mesh() {
        std::vector<Vertex> vertices_vec;
        std::vector<unsigned int> indices_vec;

        std::vector<Vertex> face1 = get_block_face_vertices(state.id, CubeFace::XNEG);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        face1 = get_block_face_vertices(state.id, CubeFace::XPOS);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        face1 = get_block_face_vertices(state.id, CubeFace::YNEG);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        face1 = get_block_face_vertices(state.id, CubeFace::YPOS);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        face1 = get_block_face_vertices(state.id, CubeFace::ZNEG);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        face1 = get_block_face_vertices(state.id, CubeFace::ZPOS);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        // block center is its, well, center, instead of bottom-left-back corner
        std::for_each(vertices_vec.begin(), vertices_vec.end(),
            [](Vertex& vert) {
                vert.position.x -= 0.5;
                vert.position.y -= 0.5;
                vert.position.z -= 0.5;
            }
        );

        std::vector<unsigned int> indices1 = get_block_face_indices(state.id, CubeFace::XNEG);
        increment_vector(indices1, 0);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
        indices1 = get_block_face_indices(state.id, CubeFace::XPOS);
        increment_vector(indices1, 4);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
        indices1 = get_block_face_indices(state.id, CubeFace::YNEG);
        increment_vector(indices1, 8);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
        indices1 = get_block_face_indices(state.id, CubeFace::YPOS);
        increment_vector(indices1, 12);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
        indices1 = get_block_face_indices(state.id, CubeFace::ZNEG);
        increment_vector(indices1, 16);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
        indices1 = get_block_face_indices(state.id, CubeFace::ZPOS);
        increment_vector(indices1, 20);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());

        mesh = Mesh(vertices_vec, indices_vec);
    }

    void increment_vector(std::vector<unsigned int>& vec, int num) {
        for (int i = 0; i < vec.size(); ++i) {
            vec[i] += num;
        }
    }
};



