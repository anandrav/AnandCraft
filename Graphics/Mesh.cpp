#include "Mesh.h"

using namespace std;

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    setup(vertices, indices);
}

// copy constructor
Mesh::Mesh(Mesh& other) {
    num_vertices = other.num_vertices;
    num_indices = other.num_indices;

    if (num_vertices == 0 || num_indices == 0) {
        VAO = 0;
        VBO = 0;
        EBO = 0;
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glCopyBufferSubData(other.VBO, VBO, 0, 0, num_vertices * sizeof(Vertex));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glCopyBufferSubData(other.EBO, EBO, 0, 0, num_indices * sizeof(unsigned int));

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

// copy assignment operator
Mesh& Mesh::operator=(Mesh& other) {
    // copy-swap
    Mesh temp(other);
    std::swap(num_vertices, temp.num_vertices);
    std::swap(num_indices, temp.num_indices);
    std::swap(VAO, temp.VAO);
    std::swap(VBO, temp.VBO);
    std::swap(EBO, temp.VBO);
    return *this;
}

// move constructor
Mesh::Mesh(Mesh&& other) noexcept {
    // transfer access to buffer data
    num_vertices = other.num_vertices;
    num_indices = other.num_indices;
    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;

    // prevent destructor of removing buffer data
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

// move assignment operator
Mesh& Mesh::operator=(Mesh&& other) noexcept {
    // move-swap
    Mesh temp(std::move(other));
    std::swap(num_vertices, temp.num_vertices);
    std::swap(num_indices, temp.num_indices);
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
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::draw_elements(int count, int start) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)*start));
    glBindVertexArray(0);
}

int Mesh::get_num_vertices() {
    return num_vertices;
}

int Mesh::get_num_indices() {
    return num_indices;
}

void Mesh::setup(const vector<Vertex>& vertices, const vector<unsigned int>& indices) {
    if (vertices.size() == 0 || indices.size() == 0) {
        VAO = 0;
        VBO = 0;
        EBO = 0;
        return;
    }

    num_vertices = (int)vertices.size();
    num_indices = (int)indices.size();

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