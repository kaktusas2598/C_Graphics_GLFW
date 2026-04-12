#ifndef SHADER_H
#define SHADER_H

#include <cglm/cglm.h>

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
void shaderSetMat4(Shader* shader, const char* name, mat4 value);
void shaderSetVec3(Shader* shader, const char* name, vec3 value);
void shaderSetFloat(Shader* shader, const char* name, float value);
void shaderSetInt(Shader* shader, const char* name, int value);

#endif
