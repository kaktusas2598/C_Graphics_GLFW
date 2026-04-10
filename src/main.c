#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <stdio.h>

#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "utils.h"

static float lastX = 400.0f;
static float lastY = 300.0f;
static int firstMouse = 1;

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);

    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = 0;
    }

    float xOffset = (float)xpos - lastX;
    float yOffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    cameraProcessMouse(camera, xOffset, -1.0f * yOffset, 1);
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Cube", NULL, NULL);
    if (!window)
        return -1;

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    Camera camera;
    cameraInit(&camera, (vec3){2.0f, 2.0f, 2.0f});
    float lastFrameTime = 0.0f;
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouseCallback);

    // -----------------------
    // Main loop
    // -----------------------
    while (!glfwWindowShouldClose(window)) {
        // Quit on Q
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        int forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
        int backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
        int left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
        int right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        cameraProcessKeyboard(&camera, forward, backward, left, right, deltaTime);

        mat4 model;
        glm_mat4_identity(model);
        glm_rotate(model, (float)glfwGetTime(), (vec3){0.0f, 1.0f, 0.0f});

        mat4 view;
        cameraGetViewMatrix(&camera, view);

        mat4 projection;
        glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

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
        shaderSetMat4(&shader, "uModel", model);
        shaderSetMat4(&shader, "uView", view);
        shaderSetMat4(&shader, "uProjection", projection);
        meshDraw(&cube);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderDestroy(&shader);
    meshDestroy(&cube);

    glfwTerminate();
    return 0;
}
