#pragma once

#include "shader.h"
#include "texture.h"

typedef struct {
    Shader* shader;
    Texture* texture;

    int useTexture;
} Material;

Material materialCreate(Shader* shader);
void materialSetTexture(Material* mat, Texture* tex);
void materialUse(Material* mat);