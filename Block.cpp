#include "Block.h"

namespace Block {

    // Lexicographical naming convention and right-hand rule used
    //      as naming convention for cube's vertices' positions.
    // (0,0,0) is the 0th coordinate (000 in binary)
    // (0,0,1) is the 1st coordinate (001 in binary)
    // (0,1,0) is the 2nd coordinate (010 in binary)
    //  etc.
    vector<Vertex> get_block_face_vertices(ID id, Face face) {
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
        case MeshType::CUBE:
            switch (face) {
            case Face::XNEG: {
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
            case Face::XPOS: {
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
            case Face::YNEG: {
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
            case Face::YPOS: {
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
            case Face::ZNEG: {
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
            case Face::ZPOS: {
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

    vector<unsigned int> get_block_face_indices(ID id, Face face) {
        vector<unsigned int> indices;
        switch (get_block_mesh_type(id)) {
        case MeshType::CUBE: {
            switch (face) {
            case Face::XNEG:
            case Face::ZNEG:
            case Face::YPOS:
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
    std::pair<int, int> get_texture_atlas_location(ID id, Face face) {
        int x, y;

        switch (id) {
        case ID::DIRT:
            x = 2;
            y = 15;
            return std::make_pair(x, y);
        case ID::GRASS:
            switch (face) {
            case Face::YPOS:
                x = 0;
                y = 15;
                return std::make_pair(x, y);
            case Face::YNEG:
                x = 2;
                y = 15;
                return std::make_pair(x, y);
            default:
                x = 3;
                y = 15;
                return std::make_pair(x, y);
                break;
            }
        case ID::STONE:
            x = 1;
            y = 15;
            return std::make_pair(x, y);
        case ID::COBBLESTONE:
            x = 0;
            y = 14;
            return std::make_pair(x, y);
        case ID::SAND:
            x = 2;
            y = 14;
            return std::make_pair(x, y);
        case ID::PLANK:
            x = 4;
            y = 15;
            return std::make_pair(x, y);
        case ID::LOG:
            switch (face) {
            case Face::YPOS:
            case Face::YNEG:
                x = 5;
                y = 14;
                return std::make_pair(x, y);
            default:
                x = 4;
                y = 14;
                return std::make_pair(x, y);
            }
        case ID::LEAF:
            x = 4;
            y = 12;
            return std::make_pair(x, y);
        case ID::BRICK:
            x = 7;
            y = 15;
            return std::make_pair(x, y);
        case ID::GLASS:
            x = 7;
            y = 7;
            return std::make_pair(x, y);
        case ID::BOOKSHELF:
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

    MeshType get_block_mesh_type(ID id) {
        switch (id) {
        case ID::AIR:
            return MeshType::NONE;
        case ID::COBBLESTONE_SLAB:
        case ID::PLANK_SLAB:
        case ID::BRICK_SLAB:
            return MeshType::SLAB;
        case ID::COBBLESTONE_STAIR:
        case ID::PLANK_STAIR:
        case ID::BRICK_STAIR:
            return MeshType::STAIR;
        case ID::ROSE:
        case ID::DAISY:
            return MeshType::X;
        default:
            return MeshType::CUBE;
        }
    }

    bool get_block_opacity(ID id) {
        switch (id) {
        case ID::AIR:
        case ID::LEAF:
        case ID::GLASS:
        case ID::ROSE:
        case ID::DAISY:
            return false;
        default:
            return true;
        }
    }

    string get_block_name(ID id) {
        switch (id) {
        case ID::AIR:
            return "Air";
        case ID::DIRT:
            return "Dirt";
        case ID::GRASS:
            return "Grass";
        case ID::STONE:
            return "Stone";
        case ID::COBBLESTONE:
            return "Cobblestone";
        case ID::SAND:
            return "Sand";
        case ID::PLANK:
            return "Plank";
        case ID::LOG:
            return "Log";
        case ID::LEAF:
            return "Leaf";
        case ID::BRICK:
            return "Brick";
        case ID::GLASS:
            return "Glass";
        case ID::BOOKSHELF:
            return "Bookshelf";

        case ID::COBBLESTONE_SLAB:
            return "Cobblestone Slab";
        case ID::PLANK_SLAB:
            return "Plank Slab";
        case ID::BRICK_SLAB:
            return "Brick Slab";

        case ID::COBBLESTONE_STAIR:
            return "Cobblestone Stair";
        case ID::PLANK_STAIR:
            return "Plank Stair";
        case ID::BRICK_STAIR:
            return "Brick Stair";

        case ID::ROSE:
            return "Rose";
        case ID::DAISY:
            return "Daisy";

        default:
            return string("no name found");
        }
    }
}