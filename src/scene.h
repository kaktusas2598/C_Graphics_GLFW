#pragma once

#include "transform.h"
#include <cglm/cglm.h>

#define MAX_ENTITIES 1024

// This is very early implementation mainly to create simple scenes for testing for now, with possible improvement in the future
typedef struct {
    Transform transform;
    vec3 color;
    float rotationSpeed;
    int active;
} Entity;

typedef struct {
    Entity entities[MAX_ENTITIES];
    int count;
} Scene;

void sceneInit(Scene* scene);
int sceneSpawnEntity(Scene* scene, vec3 position, vec3 color, float rotationSpeed);
