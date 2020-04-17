#pragma once

#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

// Thin RAII wrapper class around OpenGL vertex array object.
class Mesh {
public:
    // default constructor
    Mesh() 
    : num_indices(0)
    , VAO(0)
    , VBO(0)
    , EBO(0) 
    {
    }

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    Mesh(Mesh&& other) noexcept;

    Mesh& operator=(Mesh&& other) noexcept;

    ~Mesh();

    void draw() const;

public:
    unsigned num_indices;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

