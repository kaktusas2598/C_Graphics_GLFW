#include "mesh.h"

#include <glad/glad.h>

Mesh meshCreate(float* vertices, unsigned int vertexSize,
                unsigned int* indices, unsigned int indexCount) {

    Mesh mesh;
    mesh.indexCount = indexCount;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indexCount * sizeof(unsigned int),
                 indices,
                 GL_STATIC_DRAW);

    // position only (for now)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return mesh;
}

void meshDraw(Mesh* mesh) {
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->indexCount,
                   GL_UNSIGNED_INT, 0);
}

void meshDestroy(Mesh* mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
}
