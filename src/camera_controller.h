#pragma once

#include "camera.h"

// NOTE:
// This is a very basic camera controller intended for early development/debugging.
//
// It directly maps input → camera movement (WASD + mouse look).
// This is NOT a proper controller system.
//
// TODO:
// - Replace with a more flexible controller system (multiple modes: free-fly, orbit, follow)
// - Decouple input handling (do not pass raw input here)
// - Make it scriptable (Lua integration)
// - Possibly move to ECS (Flecs) as a system
typedef struct {
    float moveSpeed;
    float mouseSensitivity;
} CameraController;

typedef struct {
    Camera* camera;
    CameraController* controller;
} MouseContext;

// Call every frame
void cameraControllerUpdate(
    CameraController* controller,
    Camera* camera,
    float deltaTime,
    int forward, int backward,
    int left, int right
);

// Call from mouse callback
void cameraControllerMouse(
    CameraController* controller,
    Camera* camera,
    float xOffset, float yOffset,
    int constrainPitch
);
