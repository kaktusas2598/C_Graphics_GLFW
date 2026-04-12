#ifndef MESH_H
#define MESH_H

typedef struct {
    unsigned int index;      // layout location (0,1,2...)
    int componentCount;      // e.g. 3 for vec3, 2 for vec2
    unsigned int offset;     // in floats
} VertexAttribute;

#define MAX_VERTEX_ATTRIBUTES 8

typedef struct {
    int stride; // in floats
    int attributeCount;
    VertexAttribute attributes[MAX_VERTEX_ATTRIBUTES];
} MeshLayout;

typedef struct {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    unsigned int indexCount;

    MeshLayout layout; // optional for debugging
} Mesh;

Mesh meshCreate(float* vertices, unsigned int vertexSize,
                unsigned int* indices, unsigned int indexCount, MeshLayout layout);
Mesh meshCreateCube();
Mesh meshCreateTexturedCube();

void meshDraw(Mesh* mesh);
void meshDestroy(Mesh* mesh);

#endif
