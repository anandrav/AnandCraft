#pragma once

#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"

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

    Mesh(const Mesh& other);

    Mesh& operator=(const Mesh& other);

    Mesh(Mesh&& other) noexcept;

    Mesh& operator=(Mesh&& other) noexcept;

    ~Mesh();

    void draw();

    void draw_elements(int count, int start);

    int get_num_vertices();

    int get_num_indices();

private:
    int num_vertices;
    int num_indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    void setup(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};

