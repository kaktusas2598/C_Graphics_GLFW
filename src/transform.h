#pragma once

#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    vec3 rotation; // radians (x = pitch, y = yaw, z = roll)
    vec3 scale;
} Transform;

void transformInit(Transform* transform);

void transformGetMatrix(Transform* transform, mat4 dest);

void transformSetPosition(Transform* transform, vec3 position);
void transformSetRotation(Transform* transform, vec3 rotation);
void transformSetScale(Transform* transform, vec3 scale);
