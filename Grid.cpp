#include "Grid.h"

Grid::Grid() {
    const int chunk_radius = 2;
    for (int x = 0; x < chunk_radius; ++x) {
        for (int z = 0; z < chunk_radius; ++z) {
            for (int y = 0; y < chunk_radius; ++y) {
                chunks.push_back(generate_chunk(x, y, z));
            }
        }
    }

    for (int i = 0; i < chunks.size(); ++i) {
        chunks[i]->create_opaque_mesh();
        chunks[i]->create_transparent_mesh();
    }
}

void Grid::render_opaque(Camera& camera) {
    for (int i = 0; i < chunks.size(); ++i) {
        chunks[i]->render_opaque(camera);
    }
}

void Grid::render_transparent(Camera& camera) {
    glm::vec3 camera_pos = camera.get_position();
    std::sort(chunks.begin(), chunks.end(),
        [camera_pos](const GridChunk* a, const GridChunk* b) {
            // use center of chunk as location, hence CHUNK_WIDTH/2
            glm::vec3 a_pos = glm::vec3(a->x_index*CHUNK_WIDTH/2,
                a->y_index*CHUNK_HEIGHT/2, a->z_index*CHUNK_DEPTH/2);
            float a_distance = glm::length(a_pos - camera_pos);
            glm::vec3 b_pos = glm::vec3(b->x_index * CHUNK_WIDTH/2,
                b->y_index * CHUNK_HEIGHT/2, b->z_index * CHUNK_DEPTH/2);
            float b_distance = glm::length(b_pos - camera_pos);

            return a_distance > b_distance;
        }
    );

    for (int i = 0; i < chunks.size(); ++i) {
        chunks[i]->render_transparent(camera);
    }
}

bool Grid::has_block_at(int x, int y, int z) {
    int chunk_index_x = x / CHUNK_WIDTH;
    if (x < 0) { chunk_index_x -= CHUNK_WIDTH; }
    int chunk_index_y = y / CHUNK_HEIGHT;
    if (y < 0) { chunk_index_y -= CHUNK_HEIGHT; }
    int chunk_index_z = z / CHUNK_DEPTH;
    if (z < 0) { chunk_index_z -= CHUNK_DEPTH; }

    GridChunk* chunk = nullptr;
    for (int i = 0; i < chunks.size(); ++i) {
        if (chunks[i]->x_index == chunk_index_x
            && chunks[i]->y_index == chunk_index_y
            && chunks[i]->z_index == chunk_index_z) {
            chunk = chunks[i];
        }
    }

    return (chunk != nullptr);
}

Block::State Grid::get_block_at(int x, int y, int z) {
    int chunk_index_x = x / CHUNK_WIDTH;
    if (x < 0) { chunk_index_x -= CHUNK_WIDTH; }
    int chunk_index_y = y / CHUNK_HEIGHT;
    if (y < 0) { chunk_index_y -= CHUNK_HEIGHT; }
    int chunk_index_z = z / CHUNK_DEPTH;
    if (z < 0) { chunk_index_z -= CHUNK_DEPTH; }

    GridChunk* chunk = nullptr;
    for (int i = 0; i < chunks.size(); ++i) {
        if (chunks[i]->x_index == chunk_index_x
            && chunks[i]->y_index == chunk_index_y
            && chunks[i]->z_index == chunk_index_z) {
            chunk = chunks[i];
        }
    }

    if (chunk) {
        int block_coord_x = (int)x % CHUNK_WIDTH;
        int block_coord_y = (int)y % CHUNK_HEIGHT;
        int block_coord_z = (int)z % CHUNK_DEPTH;
        return chunk->data[block_coord_x][block_coord_y][block_coord_z];
    }
    throw std::invalid_argument("block does not exist");
}

Grid::~Grid() {
    for (int i = 0; i < chunks.size(); ++i) {
        delete chunks[i];
    }
}

GridChunk* Grid::generate_chunk(int x_index, int y_index, int z_index) {
    auto data = vector<vector<vector<Block::State>>>(CHUNK_WIDTH,
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
            data[x][CHUNK_HEIGHT - 7][z] = Block::State(Block::ID::GLASS);
            // 2 layers of dirt under grass
            for (int y = CHUNK_HEIGHT - 8; y >= CHUNK_HEIGHT - 9 && y >= 0; --y) {
                data[x][y][z] = Block::State(Block::ID::AIR);
            }
            // the rest is stone
        }
    }

    GridChunk* chunk = new GridChunk(x_index, y_index, z_index, data, *this);
    return chunk;
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
    shader = Shader("basic_vert.glsl", "basic_frag.glsl");
    //create_opaque_mesh();
    //create_transparent_mesh();
}

void GridChunk::render_opaque(Camera& camera) {
    shader.bind();
    glm::mat4 clip_transform = camera.get_view_projection() * transform.get_model();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, &clip_transform[0][0]);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    opaque_mesh.draw();
}

void GridChunk::render_transparent(Camera& camera) {
    shader.bind();
    glm::mat4 clip_transform = camera.get_view_projection() * transform.get_model();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, &clip_transform[0][0]);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

                switch (Block::get_mesh_type(current.id)) {
                case Block::MeshType::NONE:
                    continue;
                case Block::MeshType::CUBE:
                    // only add faces that are adjacent to transparent
                    //      blocks, cull faces that are obscured
                    if (!check_if_opaque_at(x - 1, y, z) && !check_if_same_material_at(x - 1, y, z, current)) {
                        transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size() / 6 });
                        append_block_face(vertices, indices, current,
                            Block::Face::XNEG, x, y, z);
                    }
                    if (!check_if_opaque_at(x + 1, y, z) && !check_if_same_material_at(x + 1, y, z, current)) {
                        transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size() / 6 });
                        append_block_face(vertices, indices, current,
                            Block::Face::XPOS, x, y, z);
                    }
                    if (!check_if_opaque_at(x, y - 1, z) && !check_if_same_material_at(x, y - 1, z, current)) {
                        transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size() / 6 });
                        append_block_face(vertices, indices, current,
                            Block::Face::YNEG, x, y, z);
                    }
                    if (!check_if_opaque_at(x, y + 1, z) && !check_if_same_material_at(x, y + 1, z, current)) {
                        transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size() / 6 });
                        append_block_face(vertices, indices, current,
                            Block::Face::YPOS, x, y, z);
                    }
                    if (!check_if_opaque_at(x, y, z - 1) && !check_if_same_material_at(x, y, z - 1, current)) {
                        transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size() / 6 });
                        append_block_face(vertices, indices, current,
                            Block::Face::ZNEG, x, y, z);
                    }
                    if (!check_if_opaque_at(x, y, z + 1) && !check_if_same_material_at(x, y, z + 1, current)) {
                        transparent_faces.push_back(TransparentFace{ x,y,z,(int)indices.size() / 6 });
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
            return Block::get_is_opaque(grid.get_block_at(block_x, block_y, block_z).id);
        }
        return false;
    }
    return Block::get_is_opaque(data[x][y][z].id);
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