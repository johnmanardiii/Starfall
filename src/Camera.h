#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include "Component.h"

using glm::vec3;
using glm::normalize;

class Camera : public Component
{
private:
    Camera() {}
    bool firstMouseMovement = true; //used to avoid the sudden camera movement the first time the application receives mouse movement.
    double mousePrevX, mousePrevY; //previous mouse position
    double deltaMouseX, deltaMouseY; //change in mouse position from previous callback, not necessarily previous frame.
    double xRot, yRot; //rotation in radians from start.
    vec3 w, u; //the camera basis vectors
    double sensitivityX = 0.01, sensitivityY = 0.01; //can be static for now, in the past i've used +/- keys to adjust.
public:
    //Camera(Camera const&) = delete; //to explicitly delete the copy constructor
    void update();
    void update(double posX, double posY);
    static Camera& getInstance() {
        static Camera instance;
        return instance;
    }
};

