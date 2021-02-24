#include "Block.h"

#include "SaveError.h"
#include <iostream>
#include <cassert>

using namespace std;

const unsigned int TEXTURE_ATLAS_WIDTH_IN_BLOCKS = 16;
const unsigned int TEXTURE_ATLAS_HEIGHT_IN_BLOCKS = 16;

// inline fstream& operator>>(fstream& fs, BlockID& id) {
//   unsigned int raw = 0;
//   if (fs >> raw)
//     id = static_cast<BlockID>(raw);
//   return fs;
// }

template<typename T>
void load_enum(ifstream& fs, T& t) {
    unsigned int raw = 0;
    if (fs >> raw) {
        t = static_cast<T>(raw);
    } else {
        throw SaveError("could not load block data");
    }
}

template<typename T>
void save_enum(ofstream& fs, T& t) 
{
    unsigned int raw = static_cast<unsigned int>(t);
    if (!(fs << raw)) {
        throw SaveError("could not save block data");
    }
    fs << ' ';
}

void BlockData::load(ifstream& fs) 
{
    load_enum(fs, id);
    load_enum(fs, spin);
    load_enum(fs, top_face);
}

void BlockData::save(ofstream& fs) const
{
    save_enum(fs, id);
    save_enum(fs, spin);
    save_enum(fs, top_face);
}

/*
Lexicographical naming convention and right-hand rule used as naming convention for cube vertex positions.
(0,0,0) is the 0th coordinate (000 in binary == 0 in decimal)
(0,0,1) is the 1st coordinate (001 in binary == 1 in decimal)
(0,1,0) is the 2nd coordinate (010 in binary == 2 in decimal)
(0,1,1) is the 3rd coordinate (011 in binary == 3 in decimal)
and so on.

This means that the cube is centered at (0.5,0.5,0.5),
with its "bottom left back" corner at (0,0,0)
and its "top right front" corner at (1,1,1),
using the right-hand rule.
*/
vector<Vertex> BlockData::get_face_vertices(CubeFace face) const
{
    std::pair<int, int> origin = get_texture_atlas_location(face);

    float tex_left = float(origin.first) / TEXTURE_ATLAS_WIDTH_IN_BLOCKS;
    float tex_right = float(origin.first + 1) / TEXTURE_ATLAS_WIDTH_IN_BLOCKS;
    //images use coordinate system with origin located in top left corner
    //  so y_start and y_end are negated.
    float tex_bottom = -float(origin.second) / TEXTURE_ATLAS_HEIGHT_IN_BLOCKS;
    float tex_top = -float(origin.second + 1) / TEXTURE_ATLAS_HEIGHT_IN_BLOCKS;

    switch (get_mesh_type()) {
    case BlockMesh::CUBE:
        switch (face) {
        case CubeFace::XNEG: {
            glm::vec3 normal(-1.f, 0.f, 0.f); // pointing in the negative x direction
            // bottom left back
            Vertex v000{
                glm::vec3(0.f,0.f,0.f),
                normal,
                glm::vec2(tex_left,tex_bottom) // bottom left
            };
            // bottom left front
            Vertex v001{
                glm::vec3(0.f,0.f,1.f),
                normal,
                glm::vec2(tex_right,tex_bottom) // bottom right
            };
            // top left back
            Vertex v010{
                glm::vec3(0.f,1.f,0.f),
                normal,
                glm::vec2(tex_left,tex_top) // top left
            };
            // top left front
            Vertex v011{
                glm::vec3(0.f,1.f,1.f),
                normal,
                glm::vec2(tex_right,tex_top) // top right
            };
            return { v000, v001, v010, v011 };
        }
        case CubeFace::XPOS: {
            glm::vec3 normal(1.f, 0.f, 0.f); // pointing in the positive x direction
            // bottom right back
            Vertex v100{
                glm::vec3(1.f,0.f,0.f),
                normal,
                glm::vec2(tex_right,tex_bottom) // bottom right
            };
            // bottom right front
            Vertex v101{
                glm::vec3(1.f,0.f,1.f),
                normal,
                glm::vec2(tex_left,tex_bottom) // bottom left
            };
            // top right back
            Vertex v110{
                glm::vec3(1.f,1.f,0.f),
                normal,
                glm::vec2(tex_right,tex_top) // top right
            };
            // top right front
            Vertex v111{
                glm::vec3(1.f,1.f,1.f),
                normal,
                glm::vec2(tex_left,tex_top) // top left
            };
            return { v100, v101, v110, v111 };
        }
        case CubeFace::YNEG: {
            glm::vec3 normal(0.f, -1.f, 0.f); // pointing in the negative y direction
            // bottom left back
            Vertex v000{
                glm::vec3(0.f,0.f,0.f),
                normal,
                glm::vec2(tex_left,tex_bottom) // bottom left
            };
            // bottom left front
            Vertex v001{
                glm::vec3(0.f,0.f,1.f),
                normal,
                glm::vec2(tex_left,tex_top) // top left
            };
            // bottom right back
            Vertex v100{
                glm::vec3(1.f,0.f,0.f),
                normal,
                glm::vec2(tex_right,tex_bottom) // bottom right
            };
            // bottom right front
            Vertex v101{
                glm::vec3(1.f,0.f,1.f),
                normal,
                glm::vec2(tex_right,tex_top) // top right
            };
            return { v000, v001, v100, v101 };
        }
        case CubeFace::YPOS: {
            glm::vec3 normal(0.f, 1.f, 0.f); // pointing in the positive y direction
            // top left back
            Vertex v010{
                glm::vec3(0.f,1.f,0.f),
                normal,
                glm::vec2(tex_left,tex_top) // top left
            };
            // top left front
            Vertex v011{
                glm::vec3(0.f,1.f,1.f),
                normal,
                glm::vec2(tex_left,tex_bottom) // bottom left
            };
            // top right back
            Vertex v110{
                glm::vec3(1.f,1.f,0.f),
                normal,
                glm::vec2(tex_right,tex_top) // top right
            };
            // top right front
            Vertex v111{
                glm::vec3(1.f,1.f,1.f),
                normal,
                glm::vec2(tex_right,tex_bottom) // bottom right
            };
            return { v010, v011, v110, v111 };
        }
        case CubeFace::ZNEG: {
            glm::vec3 normal(0.f, 0.f, -1.f); // pointing in the negative z direction
            // bottom left back
            Vertex v000{
                glm::vec3(0.f,0.f,0.f),
                normal,
                glm::vec2(tex_right,tex_bottom) // bottom right
            };
            // top left back
            Vertex v010{
                glm::vec3(0.f,1.f,0.f),
                normal,
                glm::vec2(tex_right,tex_top) // top right
            };
            // bottom right back
            Vertex v100{
                glm::vec3(1.f,0.f,0.f),
                normal,
                glm::vec2(tex_left,tex_bottom) // bottom left
            };
            // top right back
            Vertex v110{
                glm::vec3(1.f,1.f,0.f),
                normal,
                glm::vec2(tex_left,tex_top) // top left
            };
            return { v000, v010, v100, v110 };
        }
        case CubeFace::ZPOS: {
            glm::vec3 normal(0.f, 0.f, 1.f); // pointing in the positive z direction
            // bottom left front
            Vertex v001{
                glm::vec3(0.f,0.f,1.f),
                normal,
                glm::vec2(tex_left,tex_bottom) // bottom left
            };
            // top left front
            Vertex v011{
                glm::vec3(0.f,1.f,1.f),
                normal,
                glm::vec2(tex_left,tex_top) // top left
            };
            // bottom right front
            Vertex v101{
                glm::vec3(1.f,0.f,1.f),
                normal,
                glm::vec2(tex_right,tex_bottom) // bottom right
            };
            // top right front
            Vertex v111{
                glm::vec3(1.f,1.f,1.f),
                normal,
                glm::vec2(tex_right,tex_top) // top right
            };
            return { v001, v011, v101, v111 };
        }
        }
        break;
    default:
        assert(false);
        // prevent compiler error
        return {};
    }
    return {};
}

vector<unsigned int> BlockData::get_face_indices(CubeFace face) const
{
    // counter-clockwise triangle winding
    switch (get_mesh_type()) {
    case BlockMesh::CUBE: {
        switch (face) {
        case CubeFace::XNEG:
        case CubeFace::ZNEG:
        case CubeFace::YPOS:
            return { 
                0, 1, 2, 
                2, 1, 3 
            };
        // XPOS, ZPOS, YNEG:
        default:
            return {
                2, 1, 0,
                3, 1, 2
            };
        }

    }
    default:
        assert(false);
        // prevent compiler error
        return {};
    }
}

// return bottom left corner of texture in atlas
// coordinate system uses origin located at bottom left of image
std::pair<int, int> BlockData::get_texture_atlas_location(CubeFace face) const
{
    switch (id) {
    case BlockID::DIRT:
        return { 2, 15 };
    case BlockID::GRASS:
        switch (face) {
        case CubeFace::YPOS:
            return { 0, 15 };
        case CubeFace::YNEG:
            return { 2, 15 };
        default:
            return { 3, 15 };
        }
    case BlockID::STONE:
        return { 1, 15 };
    case BlockID::COBBLESTONE:
        return { 0, 14 };
    case BlockID::SAND:
        return { 2, 14 };
    case BlockID::PLANK:
        return { 4, 15 };
    case BlockID::LOG:
        switch (face) {
        case CubeFace::YPOS:
        case CubeFace::YNEG:
            return { 5, 14 };
        default:
            return { 4, 14 };
        }
    case BlockID::LEAF:
        return { 4, 12 };
    case BlockID::BRICK:
        return { 7, 15 };
    case BlockID::GLASS:
        return { 7, 7 };
    case BlockID::BOOKSHELF:
        return { 3, 13 };
    case BlockID::DEBUG:
        // purple debug texture
        return { 10, 0 };
    default:
        assert(false);
        // purple debug texture
        return { 10, 0 };
    }
}

BlockMesh BlockData::get_mesh_type() const
{
    switch (id) {
    case BlockID::AIR:
        return BlockMesh::NONE;

    case BlockID::COBBLESTONE_SLAB:
    case BlockID::PLANK_SLAB:
    case BlockID::BRICK_SLAB:
        return BlockMesh::SLAB;

    case BlockID::COBBLESTONE_STAIR:
    case BlockID::PLANK_STAIR:
    case BlockID::BRICK_STAIR:
        return BlockMesh::STAIR;

    case BlockID::ROSE:
    case BlockID::DAISY:
        return BlockMesh::X;

    default:
        return BlockMesh::CUBE;
    }
}

bool BlockData::is_transparent() const
{
    switch (id) {
    case BlockID::AIR:
    case BlockID::LEAF:
    case BlockID::GLASS:
    case BlockID::ROSE:
    case BlockID::DAISY:
        return true;
    default:
        return false;
    }
}

string BlockData::get_name() const
{
    switch (id) {
    case BlockID::AIR:
        return "Air";
    case BlockID::DIRT:
        return "Dirt";
    case BlockID::GRASS:
        return "Grass";
    case BlockID::STONE:
        return "Stone";
    case BlockID::COBBLESTONE:
        return "Cobblestone";
    case BlockID::SAND:
        return "Sand";
    case BlockID::PLANK:
        return "Plank";
    case BlockID::LOG:
        return "Log";
    case BlockID::LEAF:
        return "Leaf";
    case BlockID::BRICK:
        return "Brick";
    case BlockID::GLASS:
        return "Glass";
    case BlockID::BOOKSHELF:
        return "Bookshelf";
    case BlockID::DEBUG:
        return "Debug";

    case BlockID::COBBLESTONE_SLAB:
        return "Cobblestone Slab";
    case BlockID::PLANK_SLAB:
        return "Plank Slab";
    case BlockID::BRICK_SLAB:
        return "Brick Slab";

    case BlockID::COBBLESTONE_STAIR:
        return "Cobblestone Stair";
    case BlockID::PLANK_STAIR:
        return "Plank Stair";
    case BlockID::BRICK_STAIR:
        return "Brick Stair";

    case BlockID::ROSE:
        return "Rose";
    case BlockID::DAISY:
        return "Daisy";
    default:
        assert(false);
        // prevent compiler warning
        return "";
    }
}