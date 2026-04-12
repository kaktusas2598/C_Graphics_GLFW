#pragma once

#include <cglm/cglm.h>
#include "camera.h"
#include "mesh.h"
#include "material.h"

typedef struct {
    mat4 view;
    mat4 projection;
    float time;
} Renderer;

void rendererBegin(Renderer* r, Camera* camera, float time);
void rendererSetViewport(Renderer* r, int width, int height);

void rendererDrawMeshUsingMaterial(Renderer* r, Material* mat, Mesh* mesh, mat4 model, vec3 color);
void rendererDrawMeshUsingShader(Renderer* r, Shader* shader, Mesh* mesh,
                      mat4 model, vec3 color);
