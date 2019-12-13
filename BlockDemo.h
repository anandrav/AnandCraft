#pragma once

#include "Graphics/Model.h"
#include "Block.h"

class SingleBlockDemo {
public:
    SingleBlockDemo(BlockData state = BlockData(BlockID::GRASS)) : state(state) {
        setup();
    }

    void render(Camera& camera) {
        model.render(camera);
    }

    void set_block(BlockData state) {
        this->state = state;

        setup();
    }

    void set_location(glm::vec3 location) {
        model.get_transform().set_pos(location);
        transform.set_pos(location);
    }

private:
    BlockData state;
    Transform transform;
    Model model;

    void setup() {
        std::vector<Vertex> vertices_vec;
        std::vector<unsigned int> indices_vec;

        std::vector<Vertex> face1 = get_block_face_vertices(state.id, BlockFace::XNEG);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        face1 = get_block_face_vertices(state.id, BlockFace::XPOS);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        face1 = get_block_face_vertices(state.id, BlockFace::YNEG);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        face1 = get_block_face_vertices(state.id, BlockFace::YPOS);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        face1 = get_block_face_vertices(state.id, BlockFace::ZNEG);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
        face1 = get_block_face_vertices(state.id, BlockFace::ZPOS);
        vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());

        std::vector<unsigned int> indices1 = get_block_face_indices(state.id, BlockFace::XNEG);
        increment_vector(indices1, 0);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
        indices1 = get_block_face_indices(state.id, BlockFace::XPOS);
        increment_vector(indices1, 4);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
        indices1 = get_block_face_indices(state.id, BlockFace::YNEG);
        increment_vector(indices1, 8);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
        indices1 = get_block_face_indices(state.id, BlockFace::YPOS);
        increment_vector(indices1, 12);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
        indices1 = get_block_face_indices(state.id, BlockFace::ZNEG);
        increment_vector(indices1, 16);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
        indices1 = get_block_face_indices(state.id, BlockFace::ZPOS);
        increment_vector(indices1, 20);
        indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());

        Shader shader("res/basic_vert.glsl", "res/basic_frag.glsl");
        Mesh mesh(vertices_vec, indices_vec);
        model = Model(mesh, shader, transform);
    }

    void increment_vector(std::vector<unsigned int>& vec, int num) {
        for (int i = 0; i < vec.size(); ++i) {
            vec[i] += num;
        }
    }
};



