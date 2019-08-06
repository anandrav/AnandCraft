#pragma once

#include <vector>
#include <algorithm>

#include "Block.h"
#include "Camera.h"
#include "Transform.h"
#include "Shader.h"

using std::vector;

class GridChunk;

/* 3-dimensional coordinate system of blocks */
class Grid {
public:
    static const int CHUNK_WIDTH = 16;
    static const int CHUNK_HEIGHT = 16;
    static const int CHUNK_DEPTH = 16;

    Grid();

    void render_opaque(Camera& camera);

    void render_transparent(Camera& camera);

    bool has_block_at(int x, int y, int z);

    Block::State get_block_at(int x, int y, int z);

    ~Grid();

private:
    Transform transform;
    vector<GridChunk*> chunks;

    //Grid(Grid& other) {}
    //Grid(Grid&& other) noexcept {}
    //void operator=(const Grid& lhs) noexcept {}
    //void operator=(const Grid&& lhs) noexcept {}

    GridChunk* generate_chunk(int x_index, int y_index, int z_index);
};


class GridChunk {
    // Only Grid class can create and manage chunks
    friend class Grid;
private:
    const int WIDTH = Grid::CHUNK_WIDTH;
    const int HEIGHT = Grid::CHUNK_HEIGHT;
    const int DEPTH = Grid::CHUNK_DEPTH;

    // parent Grid
    Grid& grid;

    // position relative to other chunks in parent Grid
    // (CHUNK_WIDTH * x_index is x-position in world space, etc.)
    int x_index;
    int y_index;
    int z_index;

    // data for blocks in chunk, indexed by [x][y][z]
    //      x, y, and z are position within chunk
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

    GridChunk(int x_index, int y_index, int z_index,
        const vector<vector<vector<Block::State>>>& data, Grid& grid);

    void render_opaque(Camera& camera);

    void render_transparent(Camera& camera);

    void create_opaque_mesh();

    void create_transparent_mesh();

    void append_block_face(vector<Vertex>& vertices, vector<unsigned int>& indices, Block::State state,
        Block::Face face, int x, int y, int z);

    void increment_face_indices(vector<unsigned int>& vec, int num);

    void translate_vertices_in_vector(vector<Vertex>& vec, int x, int y, int z);

    bool check_if_opaque_at(int x, int y, int z);

    bool check_if_same_material_at(int x, int y, int z, Block::State current);

};
