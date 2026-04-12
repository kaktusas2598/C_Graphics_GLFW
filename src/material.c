#include "material.h"

Material materialCreate(Shader* shader) {
    Material m;
    m.shader = shader;
    m.texture = NULL;
    m.useTexture = 0;
    return m;
}

void materialSetTexture(Material* mat, Texture* tex) {
    mat->texture = tex;
    mat->useTexture = 1;
}

void materialUse(Material* mat) {
    shaderUse(mat->shader);

    if (mat->useTexture && mat->texture) {
        shaderSetInt(mat->shader, "uTexture", 0);
        textureBind(mat->texture, 0);
    }
}