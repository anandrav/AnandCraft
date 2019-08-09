#pragma once

#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"

using std::vector;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

// Thin wrapper class around OpenGL vertex array object
class Mesh {
public:
    // default constructor
    Mesh() : num_vertices(0), num_indices(0), VAO(0), VBO(0), EBO(0) {}

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    Mesh(Mesh& other);

    Mesh& operator=(Mesh& other);

    Mesh(Mesh&& other) noexcept;

    Mesh& operator=(Mesh&& other) noexcept;

    ~Mesh();

    void draw();

    void draw_elements(int count, int start);

private:
    int num_vertices;
    int num_indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    void setup(const vector<Vertex>& vertices, const vector<unsigned int>& indices);
};

