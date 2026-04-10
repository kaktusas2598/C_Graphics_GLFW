#pragma once

#include <cglm/cglm.h>
#include "transform.h"

typedef struct {
    // Transform stores world position so camera behaves like any other scene object
    Transform transform;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;

    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;
} Camera;

void cameraInit(Camera* camera, vec3 position);
void cameraUpdateVectors(Camera* camera);

void cameraProcessMouse(Camera* camera, float xOffset, float yOffset, int constrainPitch);

void cameraProcessKeyboard(Camera* camera, int forward, int backward,
                           int left, int right, float deltaTime);

void cameraGetViewMatrix(Camera* camera, mat4 dest);
