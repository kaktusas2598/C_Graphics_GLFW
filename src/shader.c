#include "shader.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

static unsigned int compileShader(unsigned int type, const char* src) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        printf("Shader compile error:\n%s\n", log);
    }

    return shader;
}

Shader shaderCreate(const char* vertexSrc, const char* fragmentSrc) {
    Shader shader;

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    shader.id = glCreateProgram();
    glAttachShader(shader.id, vs);
    glAttachShader(shader.id, fs);
    glLinkProgram(shader.id);

    int success;
    glGetProgramiv(shader.id, GL_LINK_STATUS, &success);

    if (!success) {
        char log[512];
        glGetProgramInfoLog(shader.id, 512, NULL, log);
        printf("Program link error:\n%s\n", log);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return shader;
}

Shader shaderCreateFromFile(const char* vertexPath, const char* fragmentPath) {
    char* vertexShaderSrc = loadFileToString(vertexPath);
    char* fragmentShaderSrc = loadFileToString(fragmentPath);

    if (!vertexShaderSrc || !fragmentShaderSrc) {
        printf("Failed to load shader files!\n");
        return (Shader){0};
    }

    Shader shader = shaderCreate(vertexShaderSrc, fragmentShaderSrc);

    shader.vertexPath = vertexPath;
    shader.fragmentPath = fragmentPath;

    shader.lastVertexWriteTime = getFileLastWriteTime(shader.vertexPath);
    shader.lastFragmentWriteTime = getFileLastWriteTime(shader.fragmentPath);

    return shader;
}

Shader shaderReload(Shader shader) {
    char* vertexSrc = loadFileToString(shader.vertexPath);
    char* fragmentSrc = loadFileToString(shader.fragmentPath);

    if (!vertexSrc || !fragmentSrc)
        return shader;

    unsigned int oldId = shader.id;

    Shader newShader = shaderCreate(vertexSrc, fragmentSrc);

    free(vertexSrc);
    free(fragmentSrc);

    glDeleteProgram(oldId);

    newShader.vertexPath = shader.vertexPath;
    newShader.fragmentPath = shader.fragmentPath;

    return newShader;
}

void shaderUse(Shader* shader) {
    glUseProgram(shader->id);
}

void shaderDestroy(Shader* shader) {
    glDeleteProgram(shader->id);
}

void shaderSetMat4(Shader* shader, const char* name, mat4 value) {
    int location = glGetUniformLocation(shader->id, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, (float*)value);
}

void shaderSetVec3(Shader* shader, const char* name, vec3 value) {
    int loc = glGetUniformLocation(shader->id, name);
    glUniform3fv(loc, 1, value);
}

void shaderSetFloat(Shader* shader, const char* name, float value) {
    int loc = glGetUniformLocation(shader->id, name);
    glUniform1f(loc, value);
}

void shaderSetInt(Shader* shader, const char* name, int value) {
    int loc = glGetUniformLocation(shader->id, name);
    glUniform1i(loc, value);
}