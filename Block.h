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
        // none
        AIR,

        // cube
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

        // slab
        COBBLESTONE_SLAB,
        PLANK_SLAB,
        BRICK_SLAB,

        // stair
        COBBLESTONE_STAIR,
        PLANK_STAIR,
        BRICK_STAIR,

        // X
        ROSE,
        DAISY,
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

    /* struct to store state of a block in world */

    struct State {
    public:
        State(ID id, Spin spin = Spin::NORTH, Face top_face = Face::YPOS) :
            id(id), spin(spin), top_face(top_face) { }
        ID id;
        Spin spin;
        Face top_face;
    };

    /* functions for constructing meshes with block faces */

    vector<Vertex> get_block_face_vertices(ID id, Face face);

    vector<unsigned int> get_block_face_indices(ID id, Face face);

    std::pair<int, int> get_texture_atlas_location(ID id, Face face);

    /* functions for getting block specs */

    MeshType get_mesh_type(ID id);

    bool get_is_opaque(ID id);
};