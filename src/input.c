#include "input.h"
#include <GLFW/glfw3.h>

static GLFWwindow* window = NULL;

// GLFW supports up to 512 keys safely
#define MAX_KEYS 512

static int currentKeys[MAX_KEYS];
static int previousKeys[MAX_KEYS];

void inputInit(App* app) {
    window = app->window;

    for (int i = 0; i < MAX_KEYS; i++) {
        currentKeys[i] = 0;
        previousKeys[i] = 0;
    }
}

void inputUpdate() {
    for (int key = 0; key < MAX_KEYS; key++) {
        previousKeys[key] = currentKeys[key];

        int state = glfwGetKey(window, key);
        currentKeys[key] = (state == GLFW_PRESS);
    }
}

int inputIsKeyDown(int glfwKey) {
    return currentKeys[glfwKey];
}

int inputIsKeyPressed(int glfwKey) {
    return currentKeys[glfwKey] && !previousKeys[glfwKey];
}
