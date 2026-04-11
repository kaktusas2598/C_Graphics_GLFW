#pragma once

#include "transform.h"
#include <cglm/cglm.h>

#define MAX_ENTITIES 1024

typedef struct LuaEngine LuaEngine;

// This is very early implementation mainly to create simple scenes for testing for now, with possible improvement in the future
typedef struct {
    Transform transform;
    vec3 color;
    float rotationSpeed;

    int active;
    int id;
    char name[32];
    int hasLuaUpdate;
} Entity;

typedef struct {
    Entity entities[MAX_ENTITIES];
    int count;
} Scene;

void sceneInit(Scene* scene);
int sceneSpawnEntity(Scene* scene, vec3 position, vec3 color, float rotationSpeed, const char* name);

Entity* sceneGetEntity(Scene* scene, int id);
void sceneSetRotation(Scene* scene, int id, vec3 rotation);
void sceneAddRotation(Scene* scene, int id, vec3 delta);
void sceneAddPosition(Scene* scene, int id, vec3 delta);

void sceneUpdate(Scene* scene, LuaEngine* lua, float dt);
int sceneSetLuaUpdate(Scene* scene, int id, int enabled);
