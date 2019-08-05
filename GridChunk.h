#pragma once

#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>

#include "Model.h"
#include "Block.h"

using std::vector;
using std::map;
using std::unordered_map;
using std::pair;

class GridChunk {
public:
    GridChunk(glm::vec3 location) {
         data = vector<vector<vector<Block::State>>>(CHUNK_WIDTH, 
                       vector<vector<Block::State>>(CHUNK_HEIGHT, 
                              vector<Block::State>(CHUNK_WIDTH, 
                                     Block::State(Block::ID::STONE))));
         for (int x = 0; x < CHUNK_WIDTH; ++x) {
             for (int z = 0; z < CHUNK_WIDTH; ++z) {
                 // top 5 layers air
                 for (int y = CHUNK_HEIGHT - 1; y >= CHUNK_HEIGHT - 6 && y >= 0; --y) {
                     data[x][y][z] = Block::State(Block::ID::COBBLESTONE);
                 }
                 // one layer of grass
                 data[x][CHUNK_HEIGHT-7][z] = Block::State(Block::ID::GLASS);
                 // 2 layers of dirt under grass
                 for (int y = CHUNK_HEIGHT - 8; y >= CHUNK_HEIGHT - 9 && y >= 0; --y) {
                     data[x][y][z] = Block::State(Block::ID::AIR);
                 }
                 // the rest is stone
             }
         }

         transform.set_pos(location);
         shader = Shader("basic_vert.glsl", "basic_frag.glsl");
         create_opaque_mesh();
         create_transparent_mesh();
    }

    void render_opaque(Camera& camera) {
        shader.bind();
        glm::mat4 clip_transform = camera.get_view_projection() * transform.get_model();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, &clip_transform[0][0]);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        opaque_mesh.draw();
    }

    void render_transparent(Camera& camera) {
        shader.bind();
        glm::mat4 clip_transform = camera.get_view_projection() * transform.get_model();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, &clip_transform[0][0]);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);

        glm::vec3 camera_pos = camera.get_position();
        std::sort(transparent_faces.begin(), transparent_faces.end(),
            [camera_pos](const TransparentFace& a, const TransparentFace& b) {
                glm::vec3 a_pos = glm::vec3(a.x, a.y, a.z);
                float a_distance = glm::length(a_pos - camera_pos);
                glm::vec3 b_pos = glm::vec3(b.x, b.y, b.z);
                float b_distance = glm::length(b_pos - camera_pos);

                return a_distance > b_distance;
            }
        );

        for (int i = 0; i < transparent_faces.size(); ++i) {
            transparent_mesh.draw_elements(6, transparent_faces[i].index * 6);
        }
    }

    static const unsigned int CHUNK_WIDTH = 16;
    static const unsigned int CHUNK_HEIGHT = 16;

private:
    // chunk data, indexed by [x][y][z]
    // dimensions: width x width x height
    vector<vector<vector<Block::State>>> data;

    Transform transform;
    Shader shader;
    Mesh opaque_mesh;
    Mesh transparent_mesh;
    struct TransparentFace {
        int x;
        int y;
        int z;

        int index;
    };
    vector<TransparentFace> transparent_faces;

    void create_opaque_mesh() {
        vector<Vertex> vertices;
        vector<unsigned> indices;

        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                for (int z = 0; z < CHUNK_WIDTH; ++z) {
                    Block::State& current = data[x][y][z];

                    // only render opaque blocks
                    if (!check_if_opaque_at(x, y, z)) {
                        continue;
                    }

                    switch (Block::get_mesh_type(current.id)) {
                    case Block::MeshType::NONE:
                        continue;
                    case Block::MeshType::CUBE:
                        // only add faces that are adjacent to transparent
                        //      blocks, cull faces that are obscured
                        if (!check_if_opaque_at(x - 1, y, z)) {
                            append_block_face(vertices, indices, current,
                                Block::Face::XNEG, x, y, z);
                        }
                        if (!check_if_opaque_at(x + 1, y, z)) {
                            append_block_face(vertices, indices, current,
                                Block::Face::XPOS, x, y, z);
                        }
                        if (!check_if_opaque_at(x, y - 1, z)) {
                            append_block_face(vertices, indices, current,
                                Block::Face::YNEG, x, y, z);
                        }
                        if (!check_if_opaque_at(x, y + 1, z)) {
                            append_block_face(vertices, indices, current,
                                Block::Face::YPOS, x, y, z);
                        }
                        if (!check_if_opaque_at(x, y, z - 1)) {
                            append_block_face(vertices, indices, current,
                                Block::Face::ZNEG, x, y, z);
                        }
                        if (!check_if_opaque_at(x, y, z + 1)) {
                            append_block_face(vertices, indices, current,
                                Block::Face::ZPOS, x, y, z);
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        opaque_mesh = Mesh(vertices, indices);
    }

    void create_transparent_mesh() {
        vector<Vertex> vertices;
        vector<unsigned> indices;

        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                for (int z = 0; z < CHUNK_WIDTH; ++z) {
                    Block::State& current = data[x][y][z];

                    // only render opaque blocks
                    if (check_if_opaque_at(x, y, z)) {
                        continue;
                    }
                    
                    switch (Block::get_mesh_type(current.id)) {
                    case Block::MeshType::NONE:
                        continue;
                    case Block::MeshType::CUBE:
                        // only add faces that are adjacent to transparent
                        //      blocks, cull faces that are obscured
                        if (!check_if_opaque_at(x - 1, y, z) && !check_if_same_material_at(x-1, y, z, current)) {
                            transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size()/6 });
                            append_block_face(vertices, indices, current,
                                Block::Face::XNEG, x, y, z);
                        }
                        if (!check_if_opaque_at(x + 1, y, z) && !check_if_same_material_at(x+1, y, z, current)) {
                            transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size()/6 });
                            append_block_face(vertices, indices, current,
                                Block::Face::XPOS, x, y, z);
                        }
                        if (!check_if_opaque_at(x, y - 1, z) && !check_if_same_material_at(x, y-1, z, current)) {
                            transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size()/6 });
                            append_block_face(vertices, indices, current,
                                Block::Face::YNEG, x, y, z);
                        }
                        if (!check_if_opaque_at(x, y + 1, z) && !check_if_same_material_at(x, y+1, z, current)) {
                            transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size()/6 });
                            append_block_face(vertices, indices, current,
                                Block::Face::YPOS, x, y, z);
                        }
                        if (!check_if_opaque_at(x, y, z - 1) && !check_if_same_material_at(x, y, z-1, current)) {
                            transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size()/6 });
                            append_block_face(vertices, indices, current,
                                Block::Face::ZNEG, x, y, z);
                        }
                        if (!check_if_opaque_at(x, y, z + 1) && !check_if_same_material_at(x, y, z+1, current)) {
                            transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size()/6 });
                            append_block_face(vertices, indices, current,
                                Block::Face::ZPOS, x, y, z);
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        transparent_mesh = Mesh(vertices, indices);
    }

    bool check_if_opaque_at(int x, int y, int z) {
        /* in the future, check adjacent chunks for blocks out of range */
        if (x < 0 || x >= CHUNK_WIDTH ||
            y < 0 || y >= CHUNK_HEIGHT ||
            z < 0 || z >= CHUNK_WIDTH) {
            return false;
        }
        return Block::get_is_opaque(data[x][y][z].id);
    }

    bool check_if_same_material_at(int x, int y, int z, Block::State current) {
        /* in the future, check adjacent chunks for blocks out of range */
        if (x < 0 || x >= CHUNK_WIDTH ||
            y < 0 || y >= CHUNK_HEIGHT ||
            z < 0 || z >= CHUNK_WIDTH) {
            return false;
        }
        return data[x][y][z].id == current.id;
    }

    void append_block_face(vector<Vertex>& vertices, vector<unsigned int>& indices, Block::State state,
                        Block::Face face, int x, int y, int z) {
        std::vector<unsigned int> face_indices = Block::get_block_face_indices(state.id, face);
        // adjust face_indices to point to the vertices we are about to add
        increment_face_indices(face_indices, (int)vertices.size());
        indices.insert(indices.end(), face_indices.begin(), face_indices.end());

        std::vector<Vertex> face_vertices = Block::get_block_face_vertices(state.id, face);
        // adjust face_vertices to be located at the proper position in chunk
        translate_vertices_in_vector(face_vertices, x, y, z);
        vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());
    }

    void increment_face_indices(vector<unsigned int>& vec, int num) {
        for (int i = 0; i < vec.size(); ++i) {
            vec[i] += num;
        }
    }

    void translate_vertices_in_vector(vector<Vertex>& vec, int x, int y, int z) {
        for (int i = 0; i < vec.size(); ++i) {
            vec[i].position[0] += x;
            vec[i].position[1] += y;
            vec[i].position[2] += z;
        }
    }
};

