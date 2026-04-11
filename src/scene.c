#include "scene.h"
#include "lua_engine.h"
#include <string.h>

void sceneInit(Scene* scene) {
    memset(scene, 0, sizeof(Scene));
}

int sceneSpawnEntity(Scene* scene, vec3 position, vec3 color, float rotationSpeed, const char* name) {
    if (scene->count >= MAX_ENTITIES)
        return -1;

    int id = scene->count++;
    Entity* e = &scene->entities[id];

    transformInit(&e->transform);
    glm_vec3_copy(position, e->transform.position);
    glm_vec3_copy(color, e->color);
    e->rotationSpeed = rotationSpeed;

    e->active = 1;
    e->id = id;

    strncpy(e->name, name, 31);
    e->name[31] = '\0';

    e->hasLuaUpdate = 1;

    return id;
}

Entity* sceneGetEntity(Scene* scene, int id) {
    if (id < 0 || id >= scene->count)
        return NULL;

    return &scene->entities[id];
}

void sceneSetRotation(Scene* scene, int id, vec3 rotation) {
    Entity* e = sceneGetEntity(scene, id);
    if (!e) return;

    glm_vec3_copy(rotation, e->transform.rotation);
}

void sceneAddRotation(Scene* scene, int id, vec3 delta) {
    Entity* e = sceneGetEntity(scene, id);
    if (!e) return;

    glm_vec3_add(e->transform.rotation, delta, e->transform.rotation);
}

void sceneAddPosition(Scene* scene, int id, vec3 delta) {
    Entity* e = sceneGetEntity(scene, id);
    if (!e) return;

    glm_vec3_add(e->transform.position, delta, e->transform.position);
}

void sceneUpdate(Scene* scene, LuaEngine* lua, float dt) {
    lua_State* L = (lua_State*)lua->L;

    for (int i = 0; i < scene->count; i++) {
        Entity* e = &scene->entities[i];

        if (!e->active) continue;

        if (e->hasLuaUpdate) {
            lua_getglobal(L, "updateEntity");

            if (lua_isfunction(L, -1)) {
                lua_pushinteger(L, e->id);
                lua_pushnumber(L, dt);

                if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
                    printf("Lua error: %s\n", lua_tostring(L, -1));
                    lua_pop(L, 1);
                }
            } else {
                lua_pop(L, 1);
            }
        }
    }
}

int sceneSetLuaUpdate(Scene* scene, int id, int enabled) {
    Entity* e = sceneGetEntity(scene, id);
    if (!e) return 0;

    e->hasLuaUpdate = enabled;
    return 1;
}
