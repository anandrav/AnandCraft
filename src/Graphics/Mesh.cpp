#include "Mesh.h"

using namespace std;

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
: Mesh()
{
    if (vertices.size() == 0 || indices.size() == 0)
        return;

    num_indices = unsigned(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glBindVertexArray(0);
}

// move constructor
Mesh::Mesh(Mesh&& other) noexcept 
{
    num_indices = other.num_indices;
    // transfer access to buffer data
    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;
    // prevent destructor from removing buffer data
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

// move assignment operator
Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    num_indices = other.num_indices;
    std::swap(VAO, other.VAO);
    std::swap(VBO, other.VBO);
    std::swap(EBO, other.VBO);
    return *this;
}

Mesh::~Mesh() 
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw() const
{
    if (!VAO)
        return;
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}