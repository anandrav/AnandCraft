#pragma once

#include <vector>

#include "Graphics/Mesh.h"
#include "Grid.h"

class GridChunk {
public:

    GridChunk(int x_index, int y_index, int z_index,
        const vector<vector<vector<Block::State>>>& data, Grid& grid);

    void render_opaque(Camera& camera);

    void render_transparent(Camera& camera);

    //void update_opaque_mesh();

    //void update_transparent_mesh();

    void update_opaque_mesh(Mesh&& mesh);

    void update_transparent_mesh(Mesh&& mesh);

    Block::State get_block_at(int x, int y, int z);

    void set_block_at(int x, int y, int z, Block::State new_state);

    int get_x_index();

    int get_y_index();

    int get_z_index();

    static void append_block_face(vector<Vertex>& vertices, vector<unsigned int>& indices, Block::State state,
        Block::Face face, int x, int y, int z);

    static void increment_face_indices(vector<unsigned int>& vec, int num);

    static void translate_vertices_in_vector(vector<Vertex>& vec, int x, int y, int z);

private:
    const int WIDTH = Grid::CHUNK_WIDTH;
    const int HEIGHT = Grid::CHUNK_HEIGHT;
    const int DEPTH = Grid::CHUNK_DEPTH;

    // parent Grid
    Grid& grid;

    // position relative to other chunks in parent Grid
    // (CHUNK_WIDTH * x_index is x-position in grid space, etc.)
    int x_index;
    int y_index;
    int z_index;

    // data for blocks in chunk, indexed by [x][y][z]
    //      x, y, and z are position within chunk
    vector<vector<vector<Block::State>>> data;
    std::mutex data_mutex;

    Mesh opaque_mesh;
    Mesh transparent_mesh;

    Transform transform;
    Shader shader;

    bool check_if_opaque_at(int x, int y, int z);

    bool check_if_same_material_at(int x, int y, int z, Block::State current);
};