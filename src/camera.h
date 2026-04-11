#pragma once

#include <cglm/cglm.h>
#include "transform.h"

typedef struct {
    // Transform stores world position so camera behaves like any other scene object
    Transform transform;

    float yaw;
    float pitch;

    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;
} Camera;

void cameraInit(Camera* camera, vec3 position);
void cameraUpdateVectors(Camera* camera);
void cameraGetViewMatrix(Camera* camera, mat4 dest);
