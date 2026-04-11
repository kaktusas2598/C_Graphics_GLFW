#include "scene.h"
#include <string.h>

void sceneInit(Scene* scene) {
    memset(scene, 0, sizeof(Scene));
}

int sceneSpawnEntity(Scene* scene, vec3 position, vec3 color, float rotationSpeed) {
    if (scene->count >= MAX_ENTITIES)
        return -1;

    int id = scene->count++;
    Entity* e = &scene->entities[id];

    transformInit(&e->transform);
    glm_vec3_copy(position, e->transform.position);

    glm_vec3_copy(color, e->color);
    e->rotationSpeed = rotationSpeed;
    e->active = 1;

    return id;
}
