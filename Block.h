#pragma once

#include "Graphics/Mesh.h"
#include <vector>
#include <iostream>

enum BlockID {
    // NONE mesh
    AIR,

    // CUBE mesh
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

    // SLAB mesh
    COBBLESTONE_SLAB,
    PLANK_SLAB,
    BRICK_SLAB,

    // STAIR mesh
    COBBLESTONE_STAIR,
    PLANK_STAIR,
    BRICK_STAIR,

    // X mesh
    ROSE,
    DAISY,
};

enum class BlockMesh {
    NONE,
    CUBE,
    SLAB,
    STAIR,
    X,
};

enum class BlockSpin {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

enum class BlockFace {
    XPOS,
    XNEG,
    YPOS,
    YNEG,
    ZPOS,
    ZNEG
};

/* struct to store state of a block in world */

struct BlockData {
public:
    BlockData(BlockID id, BlockSpin spin = BlockSpin::NORTH, BlockFace top_face = BlockFace::YPOS) :
        id(id), spin(spin), top_face(top_face) { }
    BlockID id;
    BlockSpin spin;
    BlockFace top_face;
};

/* functions for constructing meshes with block faces */

std::vector<Vertex> get_block_face_vertices(BlockID id, BlockFace face);

std::vector<unsigned int> get_block_face_indices(BlockID id, BlockFace face);

std::pair<int, int> get_texture_atlas_location(BlockID id, BlockFace face);

/* functions for getting block specs */

BlockMesh get_block_mesh_type(BlockID id);

bool get_block_opacity(BlockID id);

std::string get_block_name(BlockID id);