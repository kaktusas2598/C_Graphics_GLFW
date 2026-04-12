#include "texture.h"

#include <glad/glad.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture textureCreateFromFile(const char* path) {
    Texture tex;
    tex.id = 0;
    tex.width = 0;
    tex.height = 0;
    tex.channels = 0;

    stbi_set_flip_vertically_on_load(1);

    unsigned char* data = stbi_load(path,
                                     &tex.width,
                                     &tex.height,
                                     &tex.channels,
                                     0);

    if (!data) {
        printf("Failed to load texture: %s\n", path);
        return tex;
    }

    GLenum format;
    if (tex.channels == 1) format = GL_RED;
    else if (tex.channels == 3) format = GL_RGB;
    else if (tex.channels == 4) format = GL_RGBA;
    else format = GL_RGB;

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 format,
                 tex.width,
                 tex.height,
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 data);

    glGenerateMipmap(GL_TEXTURE_2D);

    // basic parameters (safe defaults)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);

    stbi_image_free(data);

    return tex;
}

void textureBind(Texture* texture, unsigned int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void textureDestroy(Texture* texture) {
    if (texture->id) {
        glDeleteTextures(1, &texture->id);
        texture->id = 0;
    }
}