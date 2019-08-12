#include "Grid.h"

Grid::Grid() : keep_running_thread(true) {
    chunk_thread = thread(&Grid::manage_chunks, this);

    const int chunk_radius = 2;
    for (int x = -1*chunk_radius; x < chunk_radius; ++x) {
        for (int z = -1*chunk_radius; z < chunk_radius; ++z) {
            for (int y = -2*chunk_radius; y < 0; ++y) {
                chunks[ChunkIndices{ x,y,z }] = generate_chunk(x, y, z);
            }
        }
    }

    for (auto& chunk : chunks) {
        chunk.second->create_opaque_mesh();
        chunk.second->create_transparent_mesh();
    }
}

void Grid::render_opaque(Camera& camera) {
    for (auto& chunk : chunks) {
        chunk.second->render_opaque(camera);
    }
}

void Grid::render_transparent(Camera& camera) {
    for (auto& chunk : chunks) {
        chunk.second->render_transparent(camera);
    }
}

bool Grid::has_block_at(int x, int y, int z) {
    return (get_chunk_at(x, y, z) != nullptr);
}

Block::State Grid::get_block_at(int x, int y, int z) {
    GridChunk* chunk = get_chunk_at(x,y,z);
    if (chunk) {
        int block_coord_x = util::positive_modulo((int)x, CHUNK_WIDTH);
        int block_coord_y = util::positive_modulo((int)y, CHUNK_HEIGHT);
        int block_coord_z = util::positive_modulo((int)z, CHUNK_DEPTH);
        return chunk->data[block_coord_x][block_coord_y][block_coord_z];
    }
    throw std::invalid_argument("block does not exist");
}

void Grid::modify_block_at(int x, int y, int z, Block::State new_state) {
    GridChunk* chunk = get_chunk_at(x, y, z);
    if (chunk) {
        int block_coord_x = util::positive_modulo((int)x, CHUNK_WIDTH);
        int block_coord_y = util::positive_modulo((int)y, CHUNK_HEIGHT);
        int block_coord_z = util::positive_modulo((int)z, CHUNK_DEPTH);
        chunk->data[block_coord_x][block_coord_y][block_coord_z] = new_state;
        chunk->create_opaque_mesh();
        chunk->create_transparent_mesh();
        return;
    }
    throw std::invalid_argument("block does not exist");
}

Grid::~Grid() {
    keep_running_thread = false;

    // join worker thread(s)
    chunk_thread.join();

    for (auto& chunk : chunks) {
        delete chunk.second;
    }
}

GridChunk* Grid::generate_chunk(int x_index, int y_index, int z_index) {
    auto data = vector<vector<vector<Block::State>>>(CHUNK_WIDTH,
                       vector<vector<Block::State>>(CHUNK_HEIGHT,
                              vector<Block::State>(CHUNK_WIDTH,
                                     Block::State(Block::ID::AIR))));

    for (int x = 0; x < CHUNK_WIDTH; ++x) {
        for (int z = 0; z < CHUNK_WIDTH; ++z) {
            // bottom 5 layers stone
            for (int y = 0; y < 5 && y < CHUNK_HEIGHT; ++y) {
                data[x][y][z] = Block::State(Block::ID::STONE);
            }
            // 2 layers of dirt under grass
            for (int y = 5; y < 7 && y < CHUNK_HEIGHT; ++y) {
                data[x][y][z] = Block::State(Block::ID::DIRT);
            }
            // one layer of grass on top
            data[x][7][z] = Block::State(Block::ID::GRASS);
        }
    }

    GridChunk* chunk = new GridChunk(x_index, y_index, z_index, data, *this);
    return chunk;
}

GridChunk* Grid::get_chunk_at(int x, int y, int z) {
    int chunk_index_x = (x >= 0) ? x / CHUNK_WIDTH
        : ((x + 1) / CHUNK_WIDTH) - 1;
    int chunk_index_y = (y >= 0) ? y / CHUNK_HEIGHT
        : ((y + 1) / CHUNK_HEIGHT) - 1;
    int chunk_index_z = (z >= 0) ? z / CHUNK_DEPTH
        : ((z + 1) / CHUNK_DEPTH) - 1;

    if (chunks.find(ChunkIndices{ chunk_index_x, chunk_index_y, chunk_index_z })
        != chunks.end()) {
        return chunks[ChunkIndices{ chunk_index_x,chunk_index_y,chunk_index_z }];
    }
    return nullptr;
}

void Grid::manage_chunks() {
    while (keep_running_thread) {
        //thread_condition.wait()
    }

    std::cout << "chunk manager no longer active" << std::endl;
}

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

    transparent_mesh.draw();
}

void GridChunk::create_opaque_mesh() {
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

    opaque_mesh = Mesh(vertices, indices);
}

void GridChunk::create_transparent_mesh() {
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

    transparent_mesh = Mesh(vertices, indices);
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
        
        int block_x = x_index * WIDTH + x;
        int block_y = y_index * HEIGHT + y;
        int block_z = z_index * DEPTH + z;

        if (grid.has_block_at(block_x, block_y, block_z)) {
            return Block::get_block_opacity(grid.get_block_at(block_x, block_y, block_z).id);
        }
        return false;
    }
    return Block::get_block_opacity(data[x][y][z].id);
}

bool GridChunk::check_if_same_material_at(int x, int y, int z, Block::State current) {
    if (x < 0 || x >= WIDTH ||
        y < 0 || y >= HEIGHT ||
        z < 0 || z >= DEPTH) {

        int block_x = x_index * WIDTH + x;
        int block_y = y_index * HEIGHT + y;
        int block_z = z_index * DEPTH + z;

        if (grid.has_block_at(block_x, block_y, block_z)) {
            return current.id == grid.get_block_at(block_x, block_y, block_z).id;
        }
        return false;
    }
    return data[x][y][z].id == current.id;
}