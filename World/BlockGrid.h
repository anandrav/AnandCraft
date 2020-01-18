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

class GridChunk;

/* 3-dimensional coordinate system of blocks */
class BlockGrid {
public:
    struct ChunkIndices;

    static const int CHUNK_WIDTH = 16;
    static const int CHUNK_HEIGHT = 16;
    static const int CHUNK_DEPTH = 16;

    BlockGrid();

    BlockGrid(BlockGrid&) = delete;
    void operator=(const BlockGrid&) = delete;

    void render_opaque(const Camera& camera);

    void render_transparent(const Camera& camera);

    void add_chunk(BlockGrid::ChunkIndices indices,
        std::vector<std::vector<std::vector<BlockState>>> data);

    bool has_chunk(BlockGrid::ChunkIndices indices);

    void remove_chunk(BlockGrid::ChunkIndices indices);

    bool has_block(int grid_x, int grid_y, int grid_z);

    BlockState get_block(int grid_x, int grid_y, int grid_z);

    void modify_block(int grid_x, int grid_y, int grid_z, BlockState new_state);

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

    ~BlockGrid();

private:
    Shader shader;
    Transform transform;

    // todo use unique_ptr or shared_ptr instead of raw pointer
    std::unordered_map<ChunkIndices, GridChunk*, ChunkIndicesHash> chunks;
    std::mutex chunks_mutex;

    GridChunk* get_chunk_at(int x, int y, int z);

    GridChunk* generate_chunk(int x_index, int y_index, int z_index);

    struct GenerateChunkJob {
        GenerateChunkJob(BlockGrid& grid, std::vector<std::vector<std::vector<BlockState>>>&& data,
            ChunkIndices indices);

        BlockGrid& grid;
        std::vector<std::vector<std::vector<BlockState>>> data;
        ChunkIndices indices;

        void operator()();
    };

    struct UpdateChunkMeshJob {
        UpdateChunkMeshJob(BlockGrid& grid, ChunkIndices indices);

        BlockGrid& grid;
        ChunkIndices indices;
        std::vector<std::vector<std::vector<BlockState>>> chunk_data_copy;
        bool cancel_job;

        void init_data_copy(GridChunk* chunk);

        void operator()();
    };

    // fixme instead of making these friends, maybe construct with references
    //      to what is necessary?
    friend struct GenerateChunkJob;
    friend struct UpdateChunkMeshJob;
};