#pragma once

#include <vector>

#include "Model.h"
#include "Block.h"

using std::vector;

class Chunk {
public:
    Chunk();

    void render();

    static const unsigned int CHUNK_WIDTH = 32;
    static const unsigned int CHUNK_HEIGHT = 32;

private:
    // chunk data, indexed by [x][y][z]
    // dimensions: width x width x height
    vector<vector<vector<Block::Data>>> data;

    Model model;
};

