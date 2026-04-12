#include "mesh.h"

#include <glad/glad.h>

Mesh meshCreate(float* vertices, unsigned int vertexSize,
                unsigned int* indices, unsigned int indexCount, MeshLayout layout) {

    Mesh mesh;
    mesh.indexCount = indexCount;
    mesh.layout = layout;

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
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
    //                       3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // Define vertex attribs using MeshLayout
    for (int i = 0; i < layout.attributeCount; i++) {
        VertexAttribute attr = layout.attributes[i];

        glVertexAttribPointer(
            attr.index,
            attr.componentCount,
            GL_FLOAT,
            GL_FALSE,
            layout.stride * sizeof(float),
            (void *)(attr.offset * sizeof(float)));

        glEnableVertexAttribArray(attr.index);
    }
    

    return mesh;
}

Mesh meshCreateCube() {
    float vertices[] = {
        -0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,

        -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f
    };

    unsigned int indices[] = {
        0,1,2, 2,3,0, // back
        4,5,6, 6,7,4, // front
        0,1,5, 5,4,0, // bottom
        3,2,6, 6,7,3, // top
        0,3,7, 7,4,0, // left
        1,2,6, 6,5,1  // right
    };

    MeshLayout layout = {0};
    layout.stride = 3;
    layout.attributeCount = 1;

    // Position - vec3 offset 0
    layout.attributes[0] = (VertexAttribute){
        .index = 0,
        .componentCount = 3,
        .offset = 0
    };

    return meshCreate(vertices, sizeof(vertices), indices, 36, layout);
}

Mesh meshCreateTexturedCube() {

    // 24 vertices (4 per face) so UVs are NOT shared correctly
    float vertices[] = {
        // back face
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,
         0.5f, 0.5f,-0.5f,  1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,

        // front face
        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,
         0.5f,-0.5f, 0.5f,  1.0f,0.0f,
         0.5f, 0.5f, 0.5f,  1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f,1.0f,

        // left
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
        -0.5f, 0.5f,-0.5f,  1.0f,0.0f,
        -0.5f, 0.5f, 0.5f,  1.0f,1.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,1.0f,

        // right
         0.5f,-0.5f,-0.5f,  0.0f,0.0f,
         0.5f, 0.5f,-0.5f,  1.0f,0.0f,
         0.5f, 0.5f, 0.5f,  1.0f,1.0f,
         0.5f,-0.5f, 0.5f,  0.0f,1.0f,

        // bottom
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,
         0.5f,-0.5f, 0.5f,  1.0f,1.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,1.0f,

        // top
        -0.5f, 0.5f,-0.5f,  0.0f,0.0f,
         0.5f, 0.5f,-0.5f,  1.0f,0.0f,
         0.5f, 0.5f, 0.5f,  1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f,1.0f,
    };

    unsigned int indices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };

        MeshLayout layout = {0};
    layout.stride = 5; // x y z u v
    layout.attributeCount = 2;

    layout.attributes[0] = (VertexAttribute){
        .index = 0,
        .componentCount = 3,
        .offset = 0
    };

    layout.attributes[1] = (VertexAttribute){
        .index = 1,
        .componentCount = 2,
        .offset = 3
    };

        return meshCreate(vertices, sizeof(vertices),
                      indices, 36,
                      layout);
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
