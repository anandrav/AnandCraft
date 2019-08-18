#include "Grid.h"
#include "GridChunk.h"

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

void Grid::add_chunk(int chunk_index_x, int chunk_index_y, int chunk_index_z,
    vector<vector<vector<Block::State>>> data) {
    GenerateChunkJob job(*this, data, chunk_index_x, chunk_index_y, chunk_index_z);
    ThreadQueue::get_instance().push(job, ThreadQueue::Priority::NORMAL);
}

bool Grid::has_chunk(int chunk_index_x, int chunk_index_y, int chunk_index_z) {
    ChunkIndices indices{ chunk_index_x, chunk_index_x, chunk_index_z };
    return chunks.find(indices) != chunks.end();
}

void Grid::remove_chunk(int chunk_index_x, int chunk_index_y, int chunk_index_z) {
    chunks.erase(ChunkIndices{ chunk_index_x, chunk_index_y, chunk_index_z });
}

bool Grid::has_block_at(int x, int y, int z) {
    return (get_chunk_at(x, y, z) != nullptr);
}

Block::State Grid::get_block_at(int x, int y, int z) {
    GridChunk* chunk = get_chunk_at(x,y,z);
    if (chunk) {
        int block_coord_x = util::positive_modulo(x, CHUNK_WIDTH);
        int block_coord_y = util::positive_modulo(y, CHUNK_HEIGHT);
        int block_coord_z = util::positive_modulo(z, CHUNK_DEPTH);
        return chunk->get_block_at(block_coord_x, block_coord_y, block_coord_z);
    }
    throw std::range_error("block does not exist");
}

void Grid::modify_block_at(int x, int y, int z, Block::State new_state) {
    GridChunk* chunk = get_chunk_at(x, y, z);
    if (chunk) {
        int block_coord_x = util::positive_modulo(x, CHUNK_WIDTH);
        int block_coord_y = util::positive_modulo(y, CHUNK_HEIGHT);
        int block_coord_z = util::positive_modulo(z, CHUNK_DEPTH);
        chunk->set_block_at(block_coord_x,block_coord_y,block_coord_z, new_state);
        UpdateChunkMeshJob job(*this, chunk);
        ThreadQueue::get_instance().push(job, ThreadQueue::Priority::HIGH);
        return;
    }
    throw std::range_error("block does not exist");
}

Grid::~Grid() {
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

    GridChunk* chunk = new GridChunk(x_index, y_index, z_index, std::move(data), *this);
    return chunk;
}

Grid::ChunkIndices Grid::get_chunk_indices_at(int x, int y, int z) {
    int chunk_index_x = (x >= 0) ? x / CHUNK_WIDTH
        : ((x + 1) / CHUNK_WIDTH) - 1;
    int chunk_index_y = (y >= 0) ? y / CHUNK_HEIGHT
        : ((y + 1) / CHUNK_HEIGHT) - 1;
    int chunk_index_z = (z >= 0) ? z / CHUNK_DEPTH
        : ((z + 1) / CHUNK_DEPTH) - 1;

    return ChunkIndices{ chunk_index_x, chunk_index_y, chunk_index_z };
}

GridChunk* Grid::get_chunk_at(int x, int y, int z) {
    ChunkIndices indices = get_chunk_indices_at(x, y, z);

    if (chunks.find(indices)
        != chunks.end()) {
        return chunks[indices];
    }
    return nullptr;
}

Grid::UpdateChunkMeshJob::UpdateChunkMeshJob(Grid& grid, GridChunk* chunk) :
    grid(grid),
    chunk(chunk) {
}

void Grid::UpdateChunkMeshJob::init_data_copy() {
    //data copy has extra 2 blocks for each dimension to hold data
    //     from adjacent chunks to determine whether to expose faces
    //     on the outside
    std::lock_guard<std::mutex> lock(grid.chunks_mutex);

    data_copy = vector<vector<vector<Block::State>>>(
        CHUNK_WIDTH + 2, vector<vector<Block::State>>(
        CHUNK_HEIGHT + 2, vector<Block::State>(
        CHUNK_WIDTH + 2, Block::State{ Block::ID::AIR })));
    for (int x = 0; x < CHUNK_WIDTH + 2; ++x) {
        for (int y = 0; y < CHUNK_HEIGHT + 2; ++y) {
            for (int z = 0; z < CHUNK_DEPTH + 2; ++z) {
                // if block from a neighboring chunk
                if (x == 0 || x == CHUNK_WIDTH + 1 ||
                    y == 0 || y == CHUNK_HEIGHT + 1 ||
                    z == 0 || z == CHUNK_DEPTH + 1) {
                    int grid_x = chunk->get_x_index() * CHUNK_WIDTH + x - 1;
                    int grid_y = chunk->get_y_index() * CHUNK_HEIGHT + y - 1;
                    int grid_z = chunk->get_z_index() * CHUNK_DEPTH + z - 1;
                    if (grid.has_block_at(grid_x, grid_y, grid_z)) {
                        data_copy[x][y][z] = grid.get_block_at(grid_x, grid_y, grid_z);
                    }
                }
                else {
                    data_copy[x][y][z] = chunk->get_block_at(x - 1, y - 1, z - 1);
                }
            }
        }
    }
}

// TODO make this way cleaner, do both meshes in ONE pass to increase the speed,
//  use helper functions, move static functions from Grid into the job.
void Grid::UpdateChunkMeshJob::operator()() {
    init_data_copy();

    vector<Vertex> opaque_vertices;
    vector<unsigned> opaque_indices;

    for (size_t x = 1; x < CHUNK_WIDTH + 1; ++x) {
        for (size_t y = 1; y < CHUNK_HEIGHT + 1; ++y) {
            for (size_t z = 1; z < CHUNK_DEPTH + 1; ++z) {
                Block::State& current = data_copy[x][y][z];

                // only render opaque blocks
                if (!Block::get_block_opacity(current.id)) {
                    continue;
                }

                switch (Block::get_block_mesh_type(current.id)) {
                case Block::MeshType::NONE:
                    continue;
                case Block::MeshType::CUBE:
                    // only add faces that are adjacent to transparent
                    //      blocks, cull faces that are obscured
                    if (!Block::get_block_opacity(data_copy[x - 1][y][z].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::XNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(data_copy[x + 1][y][z].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::XPOS, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(data_copy[x][y - 1][z].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::YNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(data_copy[x][y + 1][z].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::YPOS, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(data_copy[x][y][z - 1].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::ZNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(data_copy[x][y][z + 1].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::ZPOS, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    vector<Vertex> transparent_vertices;
    vector<unsigned> transparent_indices;

    for (size_t x = 1; x < CHUNK_WIDTH + 1; ++x) {
        for (size_t y = 1; y < CHUNK_HEIGHT + 1; ++y) {
            for (size_t z = 1; z < CHUNK_DEPTH + 1; ++z) {
                Block::State& current = data_copy[x][y][z];

                // only render transparent blocks
                if (Block::get_block_opacity(current.id)) {
                    continue;
                }

                switch (Block::get_block_mesh_type(current.id)) {
                case Block::MeshType::NONE:
                    continue;
                case Block::MeshType::CUBE:
                    // only add faces that are adjacent to transparent
                    //      blocks, cull faces that are obscured
                    if (!Block::get_block_opacity(data_copy[x - 1][y][z].id) &&
                        current.id != data_copy[x - 1][y][z].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::XNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(data_copy[x + 1][y][z].id) &&
                        current.id != data_copy[x + 1][y][z].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::XPOS, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(data_copy[x][y - 1][z].id) &&
                        current.id != data_copy[x][y - 1][z].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::YNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(data_copy[x][y + 1][z].id) &&
                        current.id != data_copy[x][y + 1][z].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::YPOS, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(data_copy[x][y][z - 1].id) &&
                        current.id != data_copy[x][y][z - 1].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::ZNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(data_copy[x][y][z + 1].id) &&
                        current.id != data_copy[x][y][z + 1].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::ZPOS, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    // Mesh creation must be done on the main thread because it contains
    //      OpenGL calls, so we send a task to AsyncQueue
    GridChunk* chunk_ptr = chunk;
    AsyncQueue::get_instance().push(
        [chunk_ptr, opaque_vertices, opaque_indices]() {
            chunk_ptr->update_opaque_mesh(Mesh(opaque_vertices, opaque_indices));
            //std::cout << "Chunk Mesh Updated\n";
        }
    );
    AsyncQueue::get_instance().push(
        [chunk_ptr, transparent_vertices, transparent_indices]() {
            chunk_ptr->update_transparent_mesh(Mesh(transparent_vertices, transparent_indices));
        }
    );
}

Grid::GenerateChunkJob::GenerateChunkJob(Grid& grid, vector<vector<vector<Block::State>>> data,
    int chunk_index_x, int chunk_index_y, int chunk_index_z) :
    grid(grid),
    data(data), 
    chunk_index_x(chunk_index_x), 
    chunk_index_y(chunk_index_y),
    chunk_index_z(chunk_index_z) {
}

void Grid::GenerateChunkJob::operator()() {
        // add chunk to grid
        GridChunk* chunk = new GridChunk(chunk_index_x, chunk_index_y, chunk_index_z, std::move(data), grid);
        AsyncQueue::get_instance().push([chunk]() {
                chunk->init_shader();
                //std::cout << "Chunk Shader Initialized\n";
            }
        );

        ChunkIndices indices{ chunk->get_x_index(), chunk->get_y_index(), chunk->get_z_index() };
        {
            std::lock_guard<std::mutex> lock(grid.chunks_mutex);
            grid.chunks[indices] = chunk;
        }

        //update the chunk's mesh (for the first time)
        UpdateChunkMeshJob job(grid, chunk);
        ThreadQueue::get_instance().push(job, ThreadQueue::Priority::NORMAL);
}