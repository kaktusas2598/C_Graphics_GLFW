#pragma once

#include "app.h"

void inputInit(App* app);

// Call once per frame
void inputUpdate();

int inputIsKeyDown(int glfwKey);
int inputIsKeyPressed(int glfwKey);
