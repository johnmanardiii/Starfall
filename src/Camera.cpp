#include "Camera.h"

void Camera::update()
{
    //TODO update based on input from mouse+keyboard
}
//not sure how to handle update based on outside parameters yet. Might refactor with event manager.
void Camera::update(double posX, double posY)
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
    float cap = glm::radians(80.0f); //in degrees
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


