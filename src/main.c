#include <cglm/cglm.h>
#include <stdio.h>

#include "camera_controller.h"
#include "input.h"
#include "scene.h"
#include "shader.h"
#include "mesh.h"
#include "utils.h"
#include "lua_engine.h"

static float lastX = 400.0f;
static float lastY = 300.0f;
static int firstMouse = 1;

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    MouseContext* context = (MouseContext*)glfwGetWindowUserPointer(window);

    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = 0;
    }

    float xOffset = (float)xpos - lastX;
    float yOffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    cameraControllerMouse(context->controller, context->camera, xOffset, yOffset, 1);
}

// Example application specific
typedef struct {
    Transform transform;
    float rotationSpeed;
    vec3 color;
} Cube;

int main() {

    App demoApplication;
    appInit(&demoApplication, 800, 600, "Cube Demo");
    inputInit(&demoApplication);

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
    Mesh cubeMesh = meshCreate(vertices, sizeof(vertices), indices, 36);

    Scene mainScene;
    sceneInit(&mainScene);

    for (int i = 0; i < 10; i++) {
        sceneSpawnEntity(&mainScene,
                         (vec3){(float)(i - 10 / 2), 0.0f, -3.0f}, //position
                         (vec3){(float)(i % 3) / 3.0f, (float)((i + 1) % 3) / 3.0f, (float)((i + 2) % 3) / 3.0f}, //colour
                         0.5f + (float)i * 0.1f,
                         "Cube"
                         );
    }

    Camera camera;
    cameraInit(&camera, (vec3){2.0f, 2.0f, 2.0f});
    CameraController cameraController = {2.5f, 0.1f};
    float lastFrameTime = 0.0f;
    MouseContext mouseContext = {&camera, &cameraController};
    glfwSetWindowUserPointer(demoApplication.window, &mouseContext);
    glfwSetCursorPosCallback(demoApplication.window, mouseCallback);

    LuaEngine luaEngine;
    luaEngineInit(&luaEngine, &camera, &mainScene);
    luaEngineRunString(&luaEngine, "print ('Hello from Lua side!')");

    long lastScriptTime = getFileLastWriteTime("scripts/script.lua");
    luaEngineRunFile(&luaEngine, "scripts/script.lua");

    // -----------------------
    // Main loop
    // -----------------------
    while (appRunning(&demoApplication)) {
        inputUpdate();

        // Script hot reloading
        long currentTime = getFileLastWriteTime("scripts/script.lua");

        if (currentTime != lastScriptTime) {
            printf("Reloading script...\n");
            luaEngineRunFile(&luaEngine, "scripts/script.lua");
            lastScriptTime = currentTime;
        }

        //Call update() method in lua script
        luaEngineRunGlobalFunction(&luaEngine, "update");

        sceneUpdate(&mainScene, &luaEngine, demoApplication.deltaTime);

        if (inputIsKeyPressed(GLFW_KEY_Q)) {
            glfwSetWindowShouldClose(demoApplication.window, 1);
        }

        int forward = inputIsKeyDown(GLFW_KEY_W);
        int backward = inputIsKeyDown(GLFW_KEY_S);
        int left = inputIsKeyDown(GLFW_KEY_A);
        int right = inputIsKeyDown(GLFW_KEY_D);

        appBeginFrame(&demoApplication);
        float currentFrameTime = demoApplication.lastTime;
        float deltaTime = demoApplication.deltaTime;

        cameraControllerUpdate(&cameraController, &camera, deltaTime, forward, backward, left, right);

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

        shaderUse(&shader);
        shaderSetFloat(&shader, "uTime", currentFrameTime);
        shaderSetMat4(&shader, "uView", view);
        shaderSetMat4(&shader, "uProjection", projection);

        for (int i = 0; i < mainScene.count; i++) {
            Entity* e = &mainScene.entities[i];
            if (!e->active) continue;

            e->transform.rotation[1] = currentFrameTime * e->rotationSpeed;

            mat4 model;
            transformGetMatrix(&e->transform, model);

            shaderSetMat4(&shader, "uModel", model);
            shaderSetVec3(&shader, "uColor", e->color);

            meshDraw(&cubeMesh);
        }


        appEndFrame(&demoApplication);
    }

    shaderDestroy(&shader);
    meshDestroy(&cubeMesh);

    luaEngineDestroy(&luaEngine);
    appShutdown(&demoApplication);
    return 0;
}
