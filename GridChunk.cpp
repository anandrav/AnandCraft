#include "GridChunk.h"

GridChunk::GridChunk(int x_index, int y_index, int z_index,
    const vector<vector<vector<Block::State>>>& data, Grid& grid) : x_index(x_index)
    , y_index(y_index)
    , z_index(z_index)
    , data(data), grid(grid) {
    glm::vec3 location = glm::vec3(x_index * WIDTH,
        y_index * HEIGHT,
        z_index * DEPTH);
    transform.set_pos(location);
    shader = Shader("res/basic_vert.glsl", "res/basic_frag.glsl");
}

void GridChunk::render_opaque(Camera& camera) {

    shader.bind();
    glm::mat4 clip_transform = camera.get_view_projection() * transform.get_model();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, &clip_transform[0][0]);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    std::lock_guard<std::mutex> lock(meshes_mutex);
    opaque_mesh.draw();
}

void GridChunk::render_transparent(Camera& camera) {
    shader.bind();
    glm::mat4 clip_transform = camera.get_view_projection() * transform.get_model();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, &clip_transform[0][0]);

    glEnable(GL_DEPTH_TEST);
    // disable depth write for transparent surfaces
    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    std::lock_guard<std::mutex> lock(meshes_mutex);
    transparent_mesh.draw();
}

void GridChunk::update_opaque_mesh() {
    vector<Vertex> vertices;
    vector<unsigned> indices;

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            for (int z = 0; z < DEPTH; ++z) {
                Block::State& current = data[x][y][z];

                // only render opaque blocks
                if (!check_if_opaque_at(x, y, z)) {
                    continue;
                }

                switch (Block::get_block_mesh_type(current.id)) {
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

    std::lock_guard<std::mutex> lock(meshes_mutex);
    opaque_mesh = Mesh(vertices, indices);
}

void GridChunk::update_transparent_mesh() {
    vector<Vertex> vertices;
    vector<unsigned> indices;

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            for (int z = 0; z < DEPTH; ++z) {
                Block::State& current = data[x][y][z];

                // only render opaque blocks
                if (check_if_opaque_at(x, y, z)) {
                    continue;
                }

                switch (Block::get_block_mesh_type(current.id)) {
                case Block::MeshType::NONE:
                    continue;
                case Block::MeshType::CUBE:
                    // only add faces that are adjacent to transparent
                    //      blocks, cull faces that are obscured
                    if (!check_if_opaque_at(x - 1, y, z) && !check_if_same_material_at(x - 1, y, z, current)) {
                        append_block_face(vertices, indices, current,
                            Block::Face::XNEG, x, y, z);
                    }
                    if (!check_if_opaque_at(x + 1, y, z) && !check_if_same_material_at(x + 1, y, z, current)) {
                        append_block_face(vertices, indices, current,
                            Block::Face::XPOS, x, y, z);
                    }
                    if (!check_if_opaque_at(x, y - 1, z) && !check_if_same_material_at(x, y - 1, z, current)) {
                        append_block_face(vertices, indices, current,
                            Block::Face::YNEG, x, y, z);
                    }
                    if (!check_if_opaque_at(x, y + 1, z) && !check_if_same_material_at(x, y + 1, z, current)) {
                        append_block_face(vertices, indices, current,
                            Block::Face::YPOS, x, y, z);
                    }
                    if (!check_if_opaque_at(x, y, z - 1) && !check_if_same_material_at(x, y, z - 1, current)) {
                        append_block_face(vertices, indices, current,
                            Block::Face::ZNEG, x, y, z);
                    }
                    if (!check_if_opaque_at(x, y, z + 1) && !check_if_same_material_at(x, y, z + 1, current)) {
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

    std::lock_guard<std::mutex> lock(meshes_mutex);
    transparent_mesh = Mesh(vertices, indices);
}

Block::State GridChunk::get_block_at(int x, int y, int z) {
    std::lock_guard<std::mutex> lock(meshes_mutex);

    return data[x][y][z];
}

void GridChunk::set_block_at(int x, int y, int z, Block::State new_state) {
    data[x][y][z] = new_state;
}

int GridChunk::get_x_index() {
    return x_index;
}

int GridChunk::get_y_index() {
    return y_index;
}

int GridChunk::get_z_index() {
    return z_index;
}

void GridChunk::append_block_face(vector<Vertex>& vertices, vector<unsigned int>& indices, Block::State state,
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

void GridChunk::increment_face_indices(vector<unsigned int>& vec, int num) {
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] += num;
    }
}

void GridChunk::translate_vertices_in_vector(vector<Vertex>& vec, int x, int y, int z) {
    for (int i = 0; i < vec.size(); ++i) {
        vec[i].position[0] += x;
        vec[i].position[1] += y;
        vec[i].position[2] += z;
    }
}

bool GridChunk::check_if_opaque_at(int x, int y, int z) {
    if (x < 0 || x >= WIDTH ||
        y < 0 || y >= HEIGHT ||
        z < 0 || z >= DEPTH) {

        int grid_x = x_index * WIDTH + x;
        int grid_y = y_index * HEIGHT + y;
        int grid_z = z_index * DEPTH + z;

        if (grid.has_block_at(grid_x, grid_y, grid_z)) {
            return Block::get_block_opacity(grid.get_block_at(grid_x, grid_y, grid_z).id);
        }
        return false;
    }
    return Block::get_block_opacity(data[x][y][z].id);
}

bool GridChunk::check_if_same_material_at(int x, int y, int z, Block::State current) {
    if (x < 0 || x >= WIDTH ||
        y < 0 || y >= HEIGHT ||
        z < 0 || z >= DEPTH) {

        int grid_x = x_index * WIDTH + x;
        int grid_y = y_index * HEIGHT + y;
        int grid_z = z_index * DEPTH + z;

        if (grid.has_block_at(grid_x, grid_y, grid_z)) {
            return current.id == grid.get_block_at(grid_x, grid_y, grid_z).id;
        }
        return false;
    }
    return data[x][y][z].id == current.id;
}