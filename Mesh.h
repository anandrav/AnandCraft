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
    Mesh() : VAO(0), VBO(0), EBO(0) {}

    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

    Mesh(Mesh& other);

    Mesh& operator=(Mesh& other);

    Mesh(Mesh&& other) noexcept;

    Mesh& operator=(Mesh&& other) noexcept;

    ~Mesh();

    void draw();

private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    void setup();
};

