#include "BlockGrid.h"
#include "GridChunk.h"

BlockGrid::BlockGrid() {
    shader = Shader("res/basic_vert.glsl", "res/basic_frag.glsl");
}

void BlockGrid::render_opaque(Camera& camera) {
    for (auto& chunk : chunks) {
        chunk.second->draw_opaque(camera, shader);
    }
}

void BlockGrid::render_transparent(Camera& camera) {
    for (auto& chunk : chunks) {
        chunk.second->draw_transparent(camera, shader);
    }
}

void BlockGrid::add_chunk(BlockGrid::ChunkIndices indices,
    vector<vector<vector<Block::State>>> data) {
    GenerateChunkJob job(*this, std::move(data), indices.x, indices.y, indices.z);
    ThreadQueue::get_instance().push(job, ThreadQueue::Priority::NORMAL);
}

bool BlockGrid::has_chunk(BlockGrid::ChunkIndices indices) {
    return chunks.find(indices) != chunks.end();
}

void BlockGrid::remove_chunk(BlockGrid::ChunkIndices indices) {
    // mutex? AsyncQueue?
    //chunks.erase(indices);
}

bool BlockGrid::has_block(int x, int y, int z) {
    return (get_chunk_at(x, y, z) != nullptr);
}

Block::State BlockGrid::get_block(int x, int y, int z) {
    GridChunk* chunk = get_chunk_at(x,y,z);
    if (chunk) {
        int block_coord_x = util::positive_modulo(x, CHUNK_WIDTH);
        int block_coord_y = util::positive_modulo(y, CHUNK_HEIGHT);
        int block_coord_z = util::positive_modulo(z, CHUNK_DEPTH);
        return chunk->get_block_at(block_coord_x, block_coord_y, block_coord_z);
    }
    throw std::range_error("block does not exist");
}

void BlockGrid::modify_block(int x, int y, int z, Block::State new_state) {
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

// convert from grid-space coordinates to indices of the chunk where they
//      are located
BlockGrid::ChunkIndices BlockGrid::get_chunk_indices(int x, int y, int z) {
    // blocks 0 to 15 are in chunk index 0
    // blocks -16 to -1 are in chunk index -1
    // and so on...
    // so negative grid-space coords must use a different formula
    int chunk_index_x = (x >= 0) ? x / CHUNK_WIDTH
        : ((x + 1) / CHUNK_WIDTH) - 1;
    int chunk_index_y = (y >= 0) ? y / CHUNK_HEIGHT
        : ((y + 1) / CHUNK_HEIGHT) - 1;
    int chunk_index_z = (z >= 0) ? z / CHUNK_DEPTH
        : ((z + 1) / CHUNK_DEPTH) - 1;

    return ChunkIndices{ chunk_index_x, chunk_index_y, chunk_index_z };
}

vector<BlockGrid::ChunkIndices> BlockGrid::get_loaded_chunks() {
    vector<ChunkIndices> loaded_chunks(chunks.size());
    int i = 0;
    for (auto it = chunks.begin(); it != chunks.end(); ++it, ++i) {
        loaded_chunks[i] = it->first;
    }
    return loaded_chunks;
}


BlockGrid::~BlockGrid() {
    for (auto& chunk : chunks) {
        delete chunk.second;
    }
}

GridChunk* BlockGrid::generate_chunk(int x_index, int y_index, int z_index) {
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

GridChunk* BlockGrid::get_chunk_at(int x, int y, int z) {
    ChunkIndices indices = get_chunk_indices(x, y, z);

    if (chunks.find(indices)
        != chunks.end()) {
        return chunks[indices];
    }
    return nullptr;
}

BlockGrid::UpdateChunkMeshJob::UpdateChunkMeshJob(BlockGrid& grid, GridChunk* chunk) :
    grid(grid),
    chunk(chunk) {
}

void BlockGrid::UpdateChunkMeshJob::init_data_copy() {
    //data copy has extra 2 blocks for each dimension to hold data
    //     from adjacent chunks to determine whether to expose faces
    //     on the outside
    chunk_data_copy = vector<vector<vector<Block::State>>>(
        CHUNK_WIDTH + 2, vector<vector<Block::State>>(
        CHUNK_HEIGHT + 2, vector<Block::State>(
        CHUNK_WIDTH + 2, Block::State{ Block::ID::AIR })));

    // since we iterate through the chunks map when we call grid.has_block and
    //  grid.get_block, we must acquire a lock to prevent our iterators from 
    //  being invalidated by the main thread
    std::lock_guard<std::mutex> lock(grid.chunks_mutex);
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
                    if (grid.has_block(grid_x, grid_y, grid_z)) {
                        chunk_data_copy[x][y][z] = grid.get_block(grid_x, grid_y, grid_z);
                    }
                }
                else {
                    chunk_data_copy[x][y][z] = chunk->get_block_at(x - 1, y - 1, z - 1);
                }
            }
        }
    }
}

// TODO make this way cleaner, do both meshes in ONE pass to increase the speed,
//  use helper functions, move static functions from Grid into the job.
void BlockGrid::UpdateChunkMeshJob::operator()() {
    init_data_copy();

    vector<Vertex> opaque_vertices;
    vector<unsigned> opaque_indices;

    for (size_t x = 1; x < CHUNK_WIDTH + 1; ++x) {
        for (size_t y = 1; y < CHUNK_HEIGHT + 1; ++y) {
            for (size_t z = 1; z < CHUNK_DEPTH + 1; ++z) {
                Block::State& current = chunk_data_copy[x][y][z];

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
                    if (!Block::get_block_opacity(chunk_data_copy[x - 1][y][z].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::XNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(chunk_data_copy[x + 1][y][z].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::XPOS, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(chunk_data_copy[x][y - 1][z].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::YNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(chunk_data_copy[x][y + 1][z].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::YPOS, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(chunk_data_copy[x][y][z - 1].id)) {
                        GridChunk::append_block_face(opaque_vertices, opaque_indices, current,
                            Block::Face::ZNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(chunk_data_copy[x][y][z + 1].id)) {
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
                Block::State& current = chunk_data_copy[x][y][z];

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
                    if (!Block::get_block_opacity(chunk_data_copy[x - 1][y][z].id) &&
                        current.id != chunk_data_copy[x - 1][y][z].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::XNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(chunk_data_copy[x + 1][y][z].id) &&
                        current.id != chunk_data_copy[x + 1][y][z].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::XPOS, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(chunk_data_copy[x][y - 1][z].id) &&
                        current.id != chunk_data_copy[x][y - 1][z].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::YNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(chunk_data_copy[x][y + 1][z].id) &&
                        current.id != chunk_data_copy[x][y + 1][z].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::YPOS, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(chunk_data_copy[x][y][z - 1].id) &&
                        current.id != chunk_data_copy[x][y][z - 1].id) {
                        GridChunk::append_block_face(transparent_vertices, transparent_indices, current,
                            Block::Face::ZNEG, (int)x - 1, (int)y - 1, (int)z - 1);
                    }
                    if (!Block::get_block_opacity(chunk_data_copy[x][y][z + 1].id) &&
                        current.id != chunk_data_copy[x][y][z + 1].id) {
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
        }
    );
    AsyncQueue::get_instance().push(
        [chunk_ptr, transparent_vertices, transparent_indices]() {
            chunk_ptr->update_transparent_mesh(Mesh(transparent_vertices, transparent_indices));
        }
    );
}

BlockGrid::GenerateChunkJob::GenerateChunkJob(BlockGrid& grid, vector<vector<vector<Block::State>>>&& data,
    int chunk_index_x, int chunk_index_y, int chunk_index_z) :
    grid(grid),
    data(std::move(data)),
    chunk_index_x(chunk_index_x), 
    chunk_index_y(chunk_index_y),
    chunk_index_z(chunk_index_z) {
}

void BlockGrid::GenerateChunkJob::operator()() {
    // add chunk to grid
    GridChunk* chunk = new GridChunk(chunk_index_x, chunk_index_y, chunk_index_z, std::move(data), grid);

    BlockGrid* grid_ptr = &grid;
    AsyncQueue::get_instance().push([grid_ptr, chunk]() {
        // add the chunk to the BlockGrid
        // we do this on the main thread so that only worker threads have
        //      to worry about iterators being invalidated, or else calling the
        //      render functions will require acquiring locks which is slow
        ChunkIndices indices{ chunk->get_x_index(), chunk->get_y_index(), chunk->get_z_index() };
        std::lock_guard<std::mutex> lock(grid_ptr->chunks_mutex);
        grid_ptr->chunks[indices] = chunk;
        }
    );

    //update the chunk's mesh (for the first time)
    BlockGrid& grid_ref = *grid_ptr;
    UpdateChunkMeshJob job(grid_ref, chunk);
    ThreadQueue::get_instance().push(job, ThreadQueue::Priority::NORMAL);
}