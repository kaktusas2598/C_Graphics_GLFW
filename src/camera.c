#include "camera.h"

#include <cglm/cglm.h>
#include <math.h>

void cameraInit(Camera* camera, vec3 position) {
    // Setup camera transform
    glm_vec3_copy(position, camera->transform.position);
    glm_vec3_zero(camera->transform.rotation);
    camera->transform.scale[0] = 1.0f;
    camera->transform.scale[1] = 1.0f;
    camera->transform.scale[2] = 1.0f;

    camera->yaw = -90.0f;
    camera->pitch = 0.0f;

    camera->worldUp[0] = 0.0f;
    camera->worldUp[1] = 1.0f;
    camera->worldUp[2] = 0.0f;

    cameraUpdateVectors(camera);
}

void cameraUpdateVectors(Camera* camera) {
    vec3 front;

    front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
    front[1] = sinf(glm_rad(camera->pitch));
    front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));

    glm_normalize_to(front, camera->front);

    glm_vec3_cross(camera->front, camera->worldUp, camera->right);
    glm_normalize(camera->right);

    glm_vec3_cross(camera->right, camera->front, camera->up);
    glm_normalize(camera->up);
}

void cameraGetViewMatrix(Camera* camera, mat4 dest) {
    vec3 center;

    glm_vec3_add(camera->transform.position, camera->front, center);
    glm_lookat(camera->transform.position, center, camera->up, dest);
}
