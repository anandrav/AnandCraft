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

    COUNT
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
    BlockData(BlockID id = BlockID::GRASS, CubeSpin spin = CubeSpin::NORTH, CubeFace top_face = CubeFace::YPOS) :
        id(id), spin(spin), top_face(top_face)
    {
    }

    std::string get_name() const;

    /* functions for constructing meshes with block faces */

    bool is_transparent() const;

    BlockMesh get_mesh_type() const;

    std::vector<Vertex> get_face_vertices(CubeFace face) const;

    std::vector<unsigned int> get_face_indices(CubeFace face) const;

    std::pair<int, int> get_texture_atlas_location(CubeFace face) const;

    BlockID id;
    // orientation
    CubeSpin spin;
    CubeFace top_face;
};