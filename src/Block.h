#pragma once

#include "Graphics/Mesh.h"
#include <vector>
#include <iosfwd>
#include <string>

enum class BlockID : uint8_t {
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
    DEBUG,

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

enum class BlockMesh : uint8_t {
    NONE,
    CUBE,
    SLAB,
    STAIR,
    X,
};

enum class CubeSpin : uint8_t {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

enum class CubeFace : uint8_t {
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
    BlockData(BlockID id = BlockID::AIR, CubeSpin spin = CubeSpin::NORTH, CubeFace top_face = CubeFace::YPOS) :
        id(id), spin(spin), top_face(top_face)
    {
    }

    BlockID id;
    // orientation fields
    CubeSpin spin;
    CubeFace top_face;
};

/* functions for constructing meshes with block faces */

std::vector<Vertex> get_block_face_vertices(BlockID id, CubeFace face);

std::vector<unsigned int> get_block_face_indices(BlockID id, CubeFace face);

std::pair<int, int> get_texture_atlas_location(BlockID id, CubeFace face);

/* functions for getting block specs */

BlockMesh get_block_mesh_type(BlockID id);

bool get_block_opacity(BlockID id);

std::string get_block_name(BlockID id);