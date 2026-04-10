#ifndef SHADER_H
#define SHADER_H

typedef struct {
    unsigned int id;

    const char* vertexPath;
    const char* fragmentPath;

    long lastVertexWriteTime;
    long lastFragmentWriteTime;
} Shader;

Shader shaderCreate(const char* vertexSrc, const char* fragmentSrc);
// Preferred method to load a shader
Shader shaderCreateFromFile(const char* vertexPath, const char* fragmentPath);
Shader shaderReload(Shader shader);
void shaderUse(Shader* shader);
void shaderDestroy(Shader* shader);

#endif
