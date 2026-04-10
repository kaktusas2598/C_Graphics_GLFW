#include "transform.h"

#include <cglm/cglm.h>

void transformInit(Transform* transform) {
    glm_vec3_zero(transform->position);
    glm_vec3_zero(transform->rotation);

    transform->scale[0] = 1.0f;
    transform->scale[1] = 1.0f;
    transform->scale[2] = 1.0f;
}

void transformGetMatrix(Transform* transform, mat4 dest) {
    glm_mat4_identity(dest);

    glm_translate(dest, transform->position);

    glm_rotate_x(dest, transform->rotation[0], dest);
    glm_rotate_y(dest, transform->rotation[1], dest);
    glm_rotate_z(dest, transform->rotation[2], dest);

    glm_scale(dest, transform->scale);
}

void transformSetPosition(Transform* transform, vec3 position) {
    glm_vec3_copy(position, transform->position);
}

void transformSetRotation(Transform* transform, vec3 rotation) {
    glm_vec3_copy(rotation, transform->rotation);
}

void transformSetScale(Transform* transform, vec3 scale) {
    glm_vec3_copy(scale, transform->scale);
}
