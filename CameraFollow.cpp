// CameraFollow.cpp
#include "CameraFollow.h"
#include "Camera.h"
extern Camera camera;
bool cameraFollow = false;
RigidBody* followTarget = nullptr;
void updateCameraFollow() {
    if (cameraFollow && followTarget)
        camera.position = followTarget->position + glm::vec3(0.0f, 2.0f, 5.0f);
}
