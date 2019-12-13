#include "Block.h"

using namespace std;

/*
Lexicographical naming convention and right-hand rule used as naming convention for cube's vertices' positions.
(0,0,0) is the 0th coordinate (000 in binary)
(0,0,1) is the 1st coordinate (001 in binary)
(0,1,0) is the 2nd coordinate (010 in binary)
etc.
This means that the cube is centered at (0.5,0.5,0.5).
*/
vector<Vertex> get_block_face_vertices(BlockID id, BlockFace face) {
    vector<Vertex> vertices;
    std::pair<int, int> location = 
        get_texture_atlas_location(id, face);
    float origin_x = (float)location.first / TEXTURE_ATLAS_WIDTH_IN_BLOCKS;
    float origin_y = (float)location.second / TEXTURE_ATLAS_HEIGHT_IN_BLOCKS;
    float unit_x = 1.f / TEXTURE_ATLAS_WIDTH_IN_BLOCKS;
    float unit_y = 1.f / TEXTURE_ATLAS_HEIGHT_IN_BLOCKS;

    // images use coordinate system with origin located in top left corner
    // flip y-axis coordinates to compensate
    origin_y = origin_y * -1;
    unit_y = unit_y * -1;

    switch (get_block_mesh_type(id)) {
    case BlockMesh::CUBE:
        switch (face) {
        case BlockFace::XNEG: {
            Vertex v000{
                glm::vec3(0.f,0.f,0.f),
                glm::vec3(-1.f,0.f,0.f),
                glm::vec2(origin_x,origin_y) // bottom left
            };
            Vertex v001{
                glm::vec3(0.f,0.f,1.f),
                glm::vec3(-1.f,0.f,0.f),
                glm::vec2(origin_x + unit_x,origin_y) // bottom right
            };
            Vertex v010{
                glm::vec3(0.f,1.f,0.f),
                glm::vec3(-1.f,0.f,0.f),
                glm::vec2(origin_x,origin_y + unit_y) // top left
            };
            Vertex v011{
                glm::vec3(0.f,1.f,1.f),
                glm::vec3(-1.f,0.f,0.f),
                glm::vec2(origin_x + unit_x,origin_y + unit_y) // top right
            };
            vertices.push_back(v000);
            vertices.push_back(v001);
            vertices.push_back(v010);
            vertices.push_back(v011);
            return vertices;
        }
        case BlockFace::XPOS: {
            Vertex v100{
                glm::vec3(1.f,0.f,0.f),
                glm::vec3(1.f,0.f,0.f),
                glm::vec2(origin_x + unit_x,origin_y) // bottom right
            };
            Vertex v101{
                glm::vec3(1.f,0.f,1.f),
                glm::vec3(1.f,0.f,0.f),
                glm::vec2(origin_x,origin_y) // bottom left
            };
            Vertex v110{
                glm::vec3(1.f,1.f,0.f),
                glm::vec3(1.f,0.f,0.f),
                glm::vec2(origin_x + unit_x,origin_y + unit_y) // top right
            };
            Vertex v111{
                glm::vec3(1.f,1.f,1.f),
                glm::vec3(1.f,0.f,0.f),
                glm::vec2(origin_x,origin_y + unit_y) // top left
            };
            vertices.push_back(v100);
            vertices.push_back(v101);
            vertices.push_back(v110);
            vertices.push_back(v111);
            return vertices;
        }
        case BlockFace::YNEG: {
            Vertex v000{
                glm::vec3(0.f,0.f,0.f),
                glm::vec3(0.f,-1.f,0.f),
                glm::vec2(origin_x,origin_y) // bottom left
            };
            Vertex v001{
                glm::vec3(0.f,0.f,1.f),
                glm::vec3(0.f,-1.f,0.f),
                glm::vec2(origin_x,origin_y + unit_y) // top left
            };
            Vertex v100{
                glm::vec3(1.f,0.f,0.f),
                glm::vec3(0.f,-1.f,0.f),
                glm::vec2(origin_x + unit_x,origin_y) // bottom right
            };
            Vertex v101{
                glm::vec3(1.f,0.f,1.f),
                glm::vec3(0.f,-1.f,0.f),
                glm::vec2(origin_x + unit_x,origin_y + unit_y) // top right
            };
            vertices.push_back(v000);
            vertices.push_back(v001);
            vertices.push_back(v100);
            vertices.push_back(v101);
            return vertices;
        }
        case BlockFace::YPOS: {
            Vertex v010{
                glm::vec3(0.f,1.f,0.f),
                glm::vec3(0.f,1.f,0.f),
                glm::vec2(origin_x,origin_y + unit_y) // top left
            };
            Vertex v011{
                glm::vec3(0.f,1.f,1.f),
                glm::vec3(0.f,1.f,0.f),
                glm::vec2(origin_x,origin_y) // bottom left
            };
            Vertex v110{
                glm::vec3(1.f,1.f,0.f),
                glm::vec3(0.f,1.f,0.f),
                glm::vec2(origin_x + unit_x,origin_y + unit_y) // top right
            };
            Vertex v111{
                glm::vec3(1.f,1.f,1.f),
                glm::vec3(0.f,1.f,0.f),
                glm::vec2(origin_x + unit_x,origin_y) // bottom right
            };
            vertices.push_back(v010);
            vertices.push_back(v011);
            vertices.push_back(v110);
            vertices.push_back(v111);
            return vertices;
        }
        case BlockFace::ZNEG: {
            Vertex v000{
                glm::vec3(0.f,0.f,0.f),
                glm::vec3(0.f,0.f,-1.f),
                glm::vec2(origin_x + unit_x,origin_y) // bottom right
            };
            Vertex v010{
                glm::vec3(0.f,1.f,0.f),
                glm::vec3(0.f,0.f,-1.f),
                glm::vec2(origin_x + unit_x,origin_y + unit_y) // top right
            };
            Vertex v100{
                glm::vec3(1.f,0.f,0.f),
                glm::vec3(0.f,0.f,-1.f),
                glm::vec2(origin_x,origin_y) // bottom left
            };
            Vertex v110{
                glm::vec3(1.f,1.f,0.f),
                glm::vec3(0.f,0.f,-1.f),
                glm::vec2(origin_x,origin_y + unit_y) // top left
            };
            vertices.push_back(v000);
            vertices.push_back(v010);
            vertices.push_back(v100);
            vertices.push_back(v110);
            return vertices;
        }
        case BlockFace::ZPOS: {
            Vertex v001{
                glm::vec3(0.f,0.f,1.f),
                glm::vec3(0.f,0.f,1.f),
                glm::vec2(origin_x,origin_y) // bottom left
            };
            Vertex v011{
                glm::vec3(0.f,1.f,1.f),
                glm::vec3(0.f,0.f,1.f),
                glm::vec2(origin_x,origin_y + unit_y) // top left
            };
            Vertex v101{
                glm::vec3(1.f,0.f,1.f),
                glm::vec3(0.f,0.f,1.f),
                glm::vec2(origin_x + unit_x,origin_y) // bottom right
            };
            Vertex v111{
                glm::vec3(1.f,1.f,1.f),
                glm::vec3(0.f,0.f,1.f),
                glm::vec2(origin_x + unit_x,origin_y + unit_y) // top right
            };
            vertices.push_back(v001);
            vertices.push_back(v011);
            vertices.push_back(v101);
            vertices.push_back(v111);
            return vertices;
        }
        }
    default:
        // prevent compiler error
        std::cout << "Block face vertices not found!" << std::endl;
        return vertices;
    }

}

vector<unsigned int> get_block_face_indices(BlockID id, BlockFace face) {
    vector<unsigned int> indices;
    switch (get_block_mesh_type(id)) {
    case BlockMesh::CUBE: {
        switch (face) {
        case BlockFace::XNEG:
        case BlockFace::ZNEG:
        case BlockFace::YPOS:
            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(2);

            indices.push_back(2);
            indices.push_back(1);
            indices.push_back(3);
            return indices;
        default:
            indices.push_back(2);
            indices.push_back(1);
            indices.push_back(0);

            indices.push_back(3);
            indices.push_back(1);
            indices.push_back(2);
            return indices;
        }

    }
    default:
        // prevent compiler error
        std::cout << "Block face indices not found!" << std::endl;
        return indices;
    }
}

// return bottom left corner of texture in atlas
// coordinate system uses origin located at bottom left of image
std::pair<int, int> get_texture_atlas_location(BlockID id, BlockFace face) {
    int x, y;

    switch (id) {
    case BlockID::DIRT:
        x = 2;
        y = 15;
        return std::make_pair(x, y);
    case BlockID::GRASS:
        switch (face) {
        case BlockFace::YPOS:
            x = 0;
            y = 15;
            return std::make_pair(x, y);
        case BlockFace::YNEG:
            x = 2;
            y = 15;
            return std::make_pair(x, y);
        default:
            x = 3;
            y = 15;
            return std::make_pair(x, y);
            break;
        }
    case BlockID::STONE:
        x = 1;
        y = 15;
        return std::make_pair(x, y);
    case BlockID::COBBLESTONE:
        x = 0;
        y = 14;
        return std::make_pair(x, y);
    case BlockID::SAND:
        x = 2;
        y = 14;
        return std::make_pair(x, y);
    case BlockID::PLANK:
        x = 4;
        y = 15;
        return std::make_pair(x, y);
    case BlockID::LOG:
        switch (face) {
        case BlockFace::YPOS:
        case BlockFace::YNEG:
            x = 5;
            y = 14;
            return std::make_pair(x, y);
        default:
            x = 4;
            y = 14;
            return std::make_pair(x, y);
        }
    case BlockID::LEAF:
        x = 4;
        y = 12;
        return std::make_pair(x, y);
    case BlockID::BRICK:
        x = 7;
        y = 15;
        return std::make_pair(x, y);
    case BlockID::GLASS:
        x = 7;
        y = 7;
        return std::make_pair(x, y);
    case BlockID::BOOKSHELF:
        x = 3;
        y = 13;
        return std::make_pair(x, y);
    default:
        // purple texture for debug purposes
        x = 10;
        y = 0;
        return std::make_pair(x, y);
    }
}

BlockMesh get_block_mesh_type(BlockID id) {
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

bool get_block_opacity(BlockID id) {
    switch (id) {
    case BlockID::AIR:
    case BlockID::LEAF:
    case BlockID::GLASS:
    case BlockID::ROSE:
    case BlockID::DAISY:
        return false;
    default:
        return true;
    }
}

string get_block_name(BlockID id) {
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
        return string("no name found");
    }
}