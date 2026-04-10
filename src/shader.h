#ifndef SHADER_H
#define SHADER_H

typedef struct {
    unsigned int id;
} Shader;

Shader shaderCreate(const char* vertexSrc, const char* fragmentSrc);
void shaderUse(Shader* shader);
void shaderDestroy(Shader* shader);

#endif
