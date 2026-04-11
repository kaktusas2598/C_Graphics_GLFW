#include "camera_controller.h"

void cameraControllerUpdate(
    CameraController* controller,
    Camera* camera,
    float deltaTime,
    int forward, int backward,
    int left, int right
) {
    float velocity = controller->moveSpeed * deltaTime;
    vec3 temp;

    if (forward) {
        glm_vec3_scale(camera->front, velocity, temp);
        glm_vec3_add(camera->transform.position, temp, camera->transform.position);
    }

    if (backward) {
        glm_vec3_scale(camera->front, velocity, temp);
        glm_vec3_sub(camera->transform.position, temp, camera->transform.position);
    }

    if (left) {
        glm_vec3_scale(camera->right, velocity, temp);
        glm_vec3_sub(camera->transform.position, temp, camera->transform.position);
    }

    if (right) {
        glm_vec3_scale(camera->right, velocity, temp);
        glm_vec3_add(camera->transform.position, temp, camera->transform.position);
    }
}

void cameraControllerMouse(
    CameraController* controller,
    Camera* camera,
    float xOffset, float yOffset,
    int constrainPitch
) {
    xOffset *= controller->mouseSensitivity;
    yOffset *= controller->mouseSensitivity;

    camera->yaw += xOffset;
    camera->pitch += yOffset;

    if (constrainPitch) {
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }

    cameraUpdateVectors(camera);
}
