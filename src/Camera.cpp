#include "Camera.h"

void Camera::ProcessWASDInput() {
    if (IsWASDPressed[0]) {
        pos -= movementSensitivity * vec3(w.x, 0.0f, w.z);
        dPos = pos;
    }
    if (IsWASDPressed[1]) {
        pos += movementSensitivity * vec3(u.x, 0.0f, u.z);
        dPos = pos;
    }
    if (IsWASDPressed[2]) {
        pos += movementSensitivity * vec3(w.x, 0.0f, w.z);
        dPos = pos;
    }
    if (IsWASDPressed[3]) {
        pos -= movementSensitivity * vec3(u.x, 0.0f, u.z);
        dPos = pos;
    }
}

void Camera::Update(float frameTime, ComponentManager& compMan)
{
    ProcessWASDInput();
    //TODO update based on input from mouse+keyboard
    vec3 target = normalize(vec3(
        cos(xRot) * cos(yRot),
        sin(yRot),
        sin(xRot) * cos(yRot)));
    vec3 up = vec3(0, 1, 0);
    mat4 lookAt = glm::lookAt(pos, pos + target, up); //first person camera. "looks at" the direction of target from the starting point of pos.
    view = lookAt; //something can watch view and do something based on that value.
}
//not sure how to handle update based on outside parameters yet. Might refactor with event manager.
void Camera::Update(double posX, double posY)
{
    if (firstMouseMovement) { //avoids first-frame jerky camera movement
        mousePrevX = posX;
        mousePrevY = posY;
        firstMouseMovement = false;
    }
    deltaMouseX = posX - mousePrevX;
    deltaMouseY = mousePrevY - posY; //y is inverted
    //adjust movement based on sensitivity
    xRot += sensitivityX * deltaMouseX;
    yRot += sensitivityY * deltaMouseY;
    //cap yRot to avoid gimbal lock.
    constexpr float cap = glm::radians<float>(80.0f); //in degrees
    if (yRot > cap) yRot = cap;
    if (yRot < -cap) yRot = -cap;
    //set the previous values
    mousePrevX = posX;
    mousePrevY = posY;
    //update gaze and cameraRight vector w and u using xRot and yRot.
    w = -normalize(vec3(
        cos(xRot) * cos(yRot),
        sin(yRot),
        sin(xRot) * cos(yRot)));
    u = cross(w, vec3(0, 1, 0)); //right is forward cross up.
}

void Camera::CalcPerspective(int width, int height)
{
    perspective = glm::perspective((float)(glm::radians(70.0f)), static_cast<float>(width) / height, 0.1f, 1000.0f);
}

