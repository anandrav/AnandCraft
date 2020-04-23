#include "BlockDemo.h"

#include "Terrain/TerrainTexture.h"

using namespace std;


SingleBlockDemo::SingleBlockDemo() : Entity("Demo"), state(BlockData(BlockID::GRASS)) {
    shader = Shader("res/basic_vert.glsl", "res/basic_frag.glsl");
    setup_mesh();
    transform.set_pos({0,1,-2});
    previous = SDL_GetTicks();
    lag = 0;
    id_index = 0;
}

void SingleBlockDemo::render_opaque(const Camera& camera) const 
{
    glBindTexture(GL_TEXTURE_2D, TerrainTexture::get());
    glUseProgram(shader.ID);
    glm::mat4 clip_transform = camera.get_view_projection() * transform.get_model();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, &clip_transform[0][0]);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    mesh.draw();
}

void SingleBlockDemo::update()
{
    transform.rotate(0.03, {0,1,0});
    // change BlockID periodically
    double current = SDL_GetTicks();
    double elapsed = current - previous;
    previous = current;
    lag += elapsed;
    const int PERIOD = 1000;
    static std::vector<BlockID> ids{BlockID::GRASS, BlockID::STONE, BlockID::COBBLESTONE, BlockID::SAND, BlockID::PLANK, BlockID::LOG, BlockID::LEAF, BlockID::BRICK, BlockID::GLASS, BlockID::BOOKSHELF, BlockID::DEBUG};
    if (lag > PERIOD) {
        lag -= PERIOD;
        id_index = (id_index + 1) % ids.size();
        state.id = ids[id_index];
        setup_mesh();
    }
}

void SingleBlockDemo::setup_mesh() {
    std::vector<Vertex> vertices_vec;
    std::vector<unsigned int> indices_vec;

    std::vector<Vertex> face1 = state.get_face_vertices(CubeFace::XNEG);
    vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
    face1 = state.get_face_vertices(CubeFace::XPOS);
    vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
    face1 = state.get_face_vertices(CubeFace::YNEG);
    vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
    face1 = state.get_face_vertices(CubeFace::YPOS);
    vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
    face1 = state.get_face_vertices(CubeFace::ZNEG);
    vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
    face1 = state.get_face_vertices(CubeFace::ZPOS);
    vertices_vec.insert(vertices_vec.end(), face1.begin(), face1.end());
    // block center is its, well, center, instead of bottom-left-back corner
    std::for_each(vertices_vec.begin(), vertices_vec.end(),
        [](Vertex& vert) {
            vert.position.x -= 0.5;
            vert.position.y -= 0.5;
            vert.position.z -= 0.5;
        }
    );

    std::vector<unsigned int> indices1 = state.get_face_indices(CubeFace::XNEG);
    increment_vector(indices1, 0);
    indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
    indices1 = state.get_face_indices(CubeFace::XPOS);
    increment_vector(indices1, 4);
    indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
    indices1 = state.get_face_indices(CubeFace::YNEG);
    increment_vector(indices1, 8);
    indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
    indices1 = state.get_face_indices(CubeFace::YPOS);
    increment_vector(indices1, 12);
    indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
    indices1 = state.get_face_indices(CubeFace::ZNEG);
    increment_vector(indices1, 16);
    indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());
    indices1 = state.get_face_indices(CubeFace::ZPOS);
    increment_vector(indices1, 20);
    indices_vec.insert(indices_vec.end(), indices1.begin(), indices1.end());

    mesh = Mesh(vertices_vec, indices_vec);
}

void SingleBlockDemo::increment_vector(std::vector<unsigned int>& vec, int num) {
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] += num;
    }
}