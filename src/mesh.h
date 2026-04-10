#ifndef MESH_H
#define MESH_H

typedef struct {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    unsigned int indexCount;
} Mesh;

Mesh meshCreate(float* vertices, unsigned int vertexSize,
                unsigned int* indices, unsigned int indexCount);

void meshDraw(Mesh* mesh);
void meshDestroy(Mesh* mesh);

#endif
