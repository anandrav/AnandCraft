#pragma once

#include "Mesh.h"
#include <vector>
#include <iostream>

using std::vector;
using std::cerr;
using std::endl;

namespace Block {

    const unsigned int TEXTURE_ATLAS_WIDTH_IN_BLOCKS = 16;
    const unsigned int TEXTURE_ATLAS_HEIGHT_IN_BLOCKS = 16;

    enum ID {
        // basic
        AIR,
        DIRT,
        GRASS,
        STONE,
        COBBLESTONE,
        SAND,
        PLANK,
        LOG,
        LEAF,
        BRICK,
        GLASS,
        BOOKSHELF,
        // flora
        ROSE,
        DAISY,
        TULIP,
    };

    enum class MeshType {
        NONE,
        CUBE,
        SLAB,
        STAIR,
        X,
    };

    enum class Spin {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    enum class Face {
        XPOS,
        XNEG,
        YPOS,
        YNEG,
        ZPOS,
        ZNEG
    };

    struct Data {
        ID id = ID::AIR;
        MeshType mesh_type = MeshType::CUBE;
        Spin spin = Spin::NORTH;
        Face top_face = Face::YPOS;
    };

    vector<Vertex> get_block_face_vertices(const Data& data, Face face);

    vector<unsigned int> get_block_face_indices(const Data& data, Face face);

    std::pair<int, int> get_texture_atlas_location(ID id, Face face);
};