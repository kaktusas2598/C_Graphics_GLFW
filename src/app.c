#include <glad/glad.h>
#include "app.h"
#include <stdio.h>

int appInit(App* app, int width, int height, const char* title) {
    if (!glfwInit()) {
        printf("Failed to init GLFW\n");
        return 0;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    app->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!app->window) {
        printf("Failed to create window\n");
        return 0;
    }

    glfwMakeContextCurrent(app->window);
    glfwFocusWindow(app->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to init GLAD\n");
        return 0;
    }

    glfwSetInputMode(app->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    app->width = width;
    app->height = height;

    app->lastTime = glfwGetTime();
    app->deltaTime = 0.0f;

    glEnable(GL_DEPTH_TEST);

    return 1;
}

int appRunning(App* app) {
    return !glfwWindowShouldClose(app->window);
}

void appBeginFrame(App* app) {
    float time = glfwGetTime();
    app->deltaTime = time - app->lastTime;
    app->lastTime = time;

    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void appEndFrame(App* app) {
    glfwSwapBuffers(app->window);
    glfwPollEvents();
}

void appShutdown(App* app) {
    glfwDestroyWindow(app->window);
    glfwTerminate();
}
