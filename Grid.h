#pragma once

#include <vector>
#include <map>
#include <algorithm>

#include "Block.h"
#include "Camera.h"
#include "Transform.h"
#include "Shader.h"
#include "util.h"

using std::vector;
using std::map;

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

    void modify_block_at(int x, int y, int z, Block::State new_state);

    ~Grid();

private:
    Transform transform;
    struct ChunkIndices {
        int x_index;
        int y_index;
        int z_index;

        bool operator==(const ChunkIndices& other) const {
            return (this->x_index == other.x_index &&
                    this->y_index == other.y_index &&
                    this->z_index == other.z_index);
        }

        bool operator<(const ChunkIndices& other) const {
            if (this->x_index != other.x_index) {
                return this->x_index < other.x_index;
            }
            if (this->y_index != other.y_index) {
                return this->y_index < other.y_index;
            }
            return this->z_index < other.z_index;
        }
    };
    map<ChunkIndices, GridChunk*> chunks;

    //Grid(Grid& other) {}
    //Grid(Grid&& other) noexcept {}
    //void operator=(const Grid& lhs) noexcept {}
    //void operator=(const Grid&& lhs) noexcept {}

    GridChunk* generate_chunk(int x_index, int y_index, int z_index);

    GridChunk* get_chunk_at(int x, int y, int z);
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
    // (CHUNK_WIDTH * x_index is x-position in grid space, etc.)
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

