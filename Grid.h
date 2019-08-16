#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

#include "Block.h"
#include "Graphics/Camera.h"
#include "Graphics/Transform.h"
#include "Graphics/Shader.h"
#include "util.h"
#include "Async/ThreadQueue.h"
#include "Async/AsyncQueue.h"

using std::vector;
using std::map;
using std::unordered_map;

class GridChunk;

/* 3-dimensional coordinate system of blocks */
class Grid {
public:
    static const int CHUNK_WIDTH = 16;
    static const int CHUNK_HEIGHT = 16;
    static const int CHUNK_DEPTH = 16;

    Grid();

    Grid(Grid&) = delete;
    void operator=(const Grid&) = delete;

    void init();

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

    struct ChunkIndicesHash {
        std::size_t operator() (const ChunkIndices& indices) const {
            std::size_t h1 = std::hash<int>()(indices.x_index);
            std::size_t h2 = std::hash<int>()(indices.y_index);
            std::size_t h3 = std::hash<int>()(indices.z_index);

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    unordered_map<ChunkIndices, GridChunk*, ChunkIndicesHash> chunks;
    std::mutex chunks_mutex;

    GridChunk* get_chunk_at(int x, int y, int z);

    GridChunk* generate_chunk(int x_index, int y_index, int z_index);

    struct UpdateChunkMeshJob {
        UpdateChunkMeshJob(Grid& grid, GridChunk* chunk);

        Grid& grid;
        GridChunk* chunk;
        vector<vector<vector<Block::State>>> data_copy;

        void init();

        void operator()();
    };
};