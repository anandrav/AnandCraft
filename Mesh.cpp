#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    this->vertices = vertices;
    this->indices = indices;

    setup();
}

// copy constructor
Mesh::Mesh(Mesh& other) {
    vertices = other.vertices;
    indices = other.indices;
    setup();
}

// copy assignment operator
Mesh& Mesh::operator=(Mesh& other) {
    // copy-swap
    Mesh temp(other);
    std::swap(vertices, temp.vertices);
    std::swap(indices, temp.indices);
    std::swap(VAO, temp.VAO);
    std::swap(VBO, temp.VBO);
    std::swap(EBO, temp.VBO);
    return *this;
}

// move constructor
Mesh::Mesh(Mesh&& other) noexcept {
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    setup();
}

// move assignment operator
Mesh& Mesh::operator=(Mesh&& other) noexcept {
    // move-swap
    Mesh temp(std::move(other));
    std::swap(vertices, temp.vertices);
    std::swap(indices, temp.indices);
    std::swap(VAO, temp.VAO);
    std::swap(VBO, temp.VBO);
    std::swap(EBO, temp.VBO);
    return *this;
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setup() {
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