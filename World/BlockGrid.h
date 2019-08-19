#pragma once

#include <vector>
#include <unordered_map>

#include "../Block.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Transform.h"
#include "../Graphics/Shader.h"
#include "../util.h"
#include "../Async/ThreadQueue.h"
#include "../Async/AsyncQueue.h"

using std::vector;
using std::unordered_map;

class GridChunk;

/* 3-dimensional coordinate system of blocks */
class BlockGrid {
public:
    struct ChunkIndices;

    static const int CHUNK_WIDTH = 32;
    static const int CHUNK_HEIGHT = 32;
    static const int CHUNK_DEPTH = 32;

    BlockGrid();

    BlockGrid(BlockGrid&) = delete;
    void operator=(const BlockGrid&) = delete;

    void render_opaque(Camera& camera);

    void render_transparent(Camera& camera);

    void add_chunk(BlockGrid::ChunkIndices indices,
        vector<vector<vector<Block::State>>> data);

    bool has_chunk(BlockGrid::ChunkIndices indices);

    void remove_chunk(BlockGrid::ChunkIndices indices);

    bool has_block(int grid_x, int grid_y, int grid_z);

    Block::State get_block(int grid_x, int grid_y, int grid_z);

    void modify_block(int grid_x, int grid_y, int grid_z, Block::State new_state);

    struct ChunkIndices {
        int x;
        int y;
        int z;

        bool operator==(const ChunkIndices& other) const {
            return (this->x == other.x &&
                this->y == other.y &&
                this->z == other.z);
        }

        bool operator<(const ChunkIndices& other) const {
            if (this->x != other.x) {
                return this->x < other.x;
            }
            if (this->y != other.y) {
                return this->y < other.y;
            }
            return this->z < other.z;
        }
    };

    struct ChunkIndicesHash {
        std::size_t operator() (const ChunkIndices& indices) const {
            std::size_t h1 = std::hash<int>()(indices.x);
            std::size_t h2 = std::hash<int>()(indices.y);
            std::size_t h3 = std::hash<int>()(indices.z);

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    static ChunkIndices get_chunk_indices(int grid_x, int grid_y, int grid_z);

    vector<ChunkIndices> get_loaded_chunks();

    ~BlockGrid();

private:
    Shader shader;
    Transform transform;

    unordered_map<ChunkIndices, GridChunk*, ChunkIndicesHash> chunks;
    std::mutex chunks_mutex;

    GridChunk* get_chunk_at(int x, int y, int z);

    GridChunk* generate_chunk(int x_index, int y_index, int z_index);

    struct GenerateChunkJob {
        GenerateChunkJob(BlockGrid& grid, vector<vector<vector<Block::State>>>&& data,
            int chunk_index_x, int chunk_index_y, int chunk_index_z);

        BlockGrid& grid;
        vector<vector<vector<Block::State>>> data;
        int chunk_index_x;
        int chunk_index_y;
        int chunk_index_z;

        void operator()();
    };

    struct UpdateChunkMeshJob {
        UpdateChunkMeshJob(BlockGrid& grid, GridChunk* chunk);

        BlockGrid& grid;
        GridChunk* chunk;
        vector<vector<vector<Block::State>>> chunk_data_copy;

        void init_data_copy();

        void operator()();
    };

    friend struct GenerateChunkJob;
    friend struct UpdateChunkMeshJob;
};