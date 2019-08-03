#pragma once

#include <vector>

#include "Model.h"
#include "Block.h"

using std::vector;

class Chunk {
public:
    Chunk() {
         data = vector<vector<vector<Block::State>>>(CHUNK_WIDTH, 
                       vector<vector<Block::State>>(CHUNK_HEIGHT, 
                              vector<Block::State>(CHUNK_WIDTH, 
                                     Block::State(Block::ID::GRASS))));

         Shader shader("basic_vert.glsl", "basic_frag.glsl");
         Mesh mesh = create_chunk_mesh();
         model = Model(mesh, shader);
    }

    void render(Camera& camera) {
        model.render(camera);
    }

    static const unsigned int CHUNK_WIDTH = 2;
    static const unsigned int CHUNK_HEIGHT = 2;

private:
    // chunk data, indexed by [x][y][z]
    // dimensions: width x width x height
    vector<vector<vector<Block::State>>> data;
    Model model;

    Mesh create_chunk_mesh() {
        vector<Vertex> vertices;
        vector<unsigned> indices;

        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                for (int z = 0; z < CHUNK_WIDTH; ++z) {
                    append_block_faces(vertices, indices, data[x][y][z], x, y, z);
                }
            }
        }

        return Mesh(vertices, indices);
    }

    void append_block_faces(vector<Vertex>& vertices, vector<unsigned int>& indices, Block::State state,
                            int x, int y, int z) {
        std::vector<Vertex> face_vertices = Block::get_block_face_vertices(state.id, Block::Face::XNEG);
        translate_vertices_in_vector(vertices, x, y, z);
        vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());
        face_vertices = Block::get_block_face_vertices(state.id, Block::Face::XPOS);
        translate_vertices_in_vector(vertices, x, y, z);
        vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());
        face_vertices = Block::get_block_face_vertices(state.id, Block::Face::YNEG);
        translate_vertices_in_vector(vertices, x, y, z);
        vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());
        face_vertices = Block::get_block_face_vertices(state.id, Block::Face::YPOS);
        translate_vertices_in_vector(vertices, x, y, z);
        vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());
        face_vertices = Block::get_block_face_vertices(state.id, Block::Face::ZNEG);
        translate_vertices_in_vector(vertices, x, y, z);
        vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());
        face_vertices = Block::get_block_face_vertices(state.id, Block::Face::ZPOS);
        translate_vertices_in_vector(vertices, x, y, z);
        vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());

        std::vector<unsigned int> face_indices = Block::get_block_face_indices(state.id, Block::Face::XNEG);
        increment_vector(face_indices, 0 + vertices.size());
        indices.insert(indices.end(), face_indices.begin(), face_indices.end());
        face_indices = Block::get_block_face_indices(state.id, Block::Face::XPOS);
        increment_vector(face_indices, 4 + vertices.size());
        indices.insert(indices.end(), face_indices.begin(), face_indices.end());
        face_indices = Block::get_block_face_indices(state.id, Block::Face::YNEG);
        increment_vector(face_indices, 8 + vertices.size());
        indices.insert(indices.end(), face_indices.begin(), face_indices.end());
        face_indices = Block::get_block_face_indices(state.id, Block::Face::YPOS);
        increment_vector(face_indices, 12 + vertices.size());
        indices.insert(indices.end(), face_indices.begin(), face_indices.end());
        face_indices = Block::get_block_face_indices(state.id, Block::Face::ZNEG);
        increment_vector(face_indices, 16 + vertices.size());
        indices.insert(indices.end(), face_indices.begin(), face_indices.end());
        face_indices = Block::get_block_face_indices(state.id, Block::Face::ZPOS);
        increment_vector(face_indices, 20 + vertices.size());
        indices.insert(indices.end(), face_indices.begin(), face_indices.end());
    }

    void increment_vector(vector<unsigned int>& vec, int num) {
        for (int i = 0; i < vec.size(); ++i) {
            vec[i] += num;
        }
    }

    void translate_vertices_in_vector(vector<Vertex>& vec, int x, int y, int z) {
        x = 1;
        y = 1;
        z = 1;
        for (int i = 0; i < vec.size(); ++i) {
            vec[i].position[0] += x;
            vec[i].position[1] += y;
            vec[i].position[2] += z;
        }
    }
};

