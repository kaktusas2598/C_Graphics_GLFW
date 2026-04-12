#pragma once

typedef struct {
    unsigned int id;
    int width;
    int height;
    int channels;
} Texture;

Texture textureCreateFromFile(const char* path);
void textureBind(Texture* texture, unsigned int slot);
void textureDestroy(Texture* texture);