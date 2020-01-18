#pragma once

#include "../Graphics/Mesh.h"
#include "BlockGrid.h"

class GridChunk {
public:

    GridChunk(int x_index, int y_index, int z_index,
        const std::vector<std::vector<std::vector<BlockState>>>&& data, BlockGrid& grid);

    void draw_opaque(const Camera& camera, Shader& shader);

    void draw_transparent(const Camera& camera, Shader& shader);

    void update_opaque_mesh(Mesh&& mesh);

    void update_transparent_mesh(Mesh&& mesh);

    BlockState get_block_at(int x, int y, int z);

    void set_block_at(int x, int y, int z, BlockState new_state);

    int get_x_index();

    int get_y_index();

    int get_z_index();

    static void append_block_face(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, BlockState state,
        BlockFace face, int x, int y, int z);

    static void increment_face_indices(std::vector<unsigned int>& vec, int num);

    static void translate_vertices_in_vector(std::vector<Vertex>& vec, int x, int y, int z);

private:
    const int WIDTH = BlockGrid::CHUNK_WIDTH;
    const int HEIGHT = BlockGrid::CHUNK_HEIGHT;
    const int DEPTH = BlockGrid::CHUNK_DEPTH;

    // parent Grid
    BlockGrid& grid;

    // position relative to other chunks in parent Grid
    // (CHUNK_WIDTH * x_index is x-position in grid space, etc.)
    int x_index;
    int y_index;
    int z_index;

    // data for blocks in chunk, indexed by [x][y][z]
    //      x, y, and z are position within chunk
    std::vector<std::vector<std::vector<BlockState>>> data;
    std::mutex mutex;

    Mesh opaque_mesh;
    Mesh transparent_mesh;

    Transform transform;

    bool check_if_opaque_at(int x, int y, int z);

    bool check_if_same_material_at(int x, int y, int z, BlockState current);
};