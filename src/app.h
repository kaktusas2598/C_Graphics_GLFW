#pragma once

#include <GLFW/glfw3.h>

typedef struct {
    GLFWwindow* window;

    int width;
    int height;

    float deltaTime;
    float lastTime;
} App;

int appInit(App* app, int width, int height, const char* title);

int appRunning(App* app);

void appBeginFrame(App* app);
void appEndFrame(App* app);

void appShutdown(App* app);
