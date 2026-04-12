#include "renderer.h"
#include "shader.h"

void rendererBegin(Renderer* r, Camera* camera, float time) {
    r->time = time;

    cameraGetViewMatrix(camera, r->view);

    // TODO: later move this into window system ?
    glm_perspective(glm_rad(45.0f),
                    800.0f / 600.0f,
                    0.1f,
                    100.0f,
                    r->projection);
}


void rendererSetViewport(Renderer* r, int width, int height) {
    // TODO: implement
}

void rendererDrawMeshUsingShader(Renderer* r,
                      Shader* shader,
                      Mesh* mesh,
                      mat4 model,
                      vec3 color) {

    shaderUse(shader);

    // GLOBAL uniforms (renderer owns these)
    shaderSetMat4(shader, "uView", r->view);
    shaderSetMat4(shader, "uProjection", r->projection);
    shaderSetFloat(shader, "uTime", r->time);

    // PER OBJECT uniforms
    shaderSetMat4(shader, "uModel", model);
    shaderSetVec3(shader, "uColor", color);

    meshDraw(mesh);
}

void rendererDrawMeshUsingMaterial(Renderer* r, Material* mat, Mesh* mesh, mat4 model, vec3 color) {

    materialUse(mat);

    Shader* shader = mat->shader;

    shaderSetMat4(shader, "uView", r->view);
    shaderSetMat4(shader, "uProjection", r->projection);
    shaderSetFloat(shader, "uTime", r->time);

    shaderSetMat4(shader, "uModel", model);
    shaderSetVec3(shader, "uColor", color);

    meshDraw(mesh);
}