#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <stdio.h>

#include "shader.h"
#include "mesh.h"
#include "utils.h"

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Cube", NULL, NULL);
    if (!window)
        return -1;

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;

    glEnable(GL_DEPTH_TEST);

    // -----------------------
    // Cube vertices
    // -----------------------
    float vertices[] = {
        -0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,

        -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f
    };

    unsigned int indices[] = {
        0,1,2, 2,3,0, // back
        4,5,6, 6,7,4, // front
        0,1,5, 5,4,0, // bottom
        3,2,6, 6,7,3, // top
        0,3,7, 7,4,0, // left
        1,2,6, 6,5,1  // right
    };

    Shader shader = shaderCreateFromFile("shaders/basic.vert", "shaders/basic.frag");
    Mesh cube = meshCreate(vertices, sizeof(vertices), indices, 36);

    // -----------------------
    // Main loop
    // -----------------------
    while (!glfwWindowShouldClose(window)) {
        // Quit on Q
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        long vTime = getFileLastWriteTime(shader.vertexPath);
        long fTime = getFileLastWriteTime(shader.fragmentPath);

        // Hot Reloading - Check if shaders have to be reloaded
        if (vTime != shader.lastVertexWriteTime ||
            fTime != shader.lastFragmentWriteTime) {

            shader = shaderReload(shader);

            shader.lastVertexWriteTime = vTime;
            shader.lastFragmentWriteTime = fTime;
        }


        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderUse(&shader);
        meshDraw(&cube);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
