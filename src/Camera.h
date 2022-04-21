#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //for lookat
#include <glm/gtc/type_ptr.hpp> //for value_ptr

#include <cmath>

#include "Component.h"
#include "WindowManager.h"

using glm::vec3;
using glm::mat4;
using glm::normalize;


class Camera : public Component
{
private:
    Camera(vec3 pos) : Component("Camera"), pos(pos) {}
    vec3 pos = vec3(0); //starts at 0,0,0 for now. TODO move into move component
    vec3 dPos = vec3(0);
    double xRot = 0.0, yRot = 0.0; //rotation in radians from start. TODO move into move component, this or the basis vectors, or something else to describe rotate orientation.
    vec3 w = vec3(0), u = vec3(0); //the camera basis vectors TODO move into move component
    mat4 view = mat4(1.0f);
    mat4 perspective = mat4(1.0f);
    
    float movementSensitivity = 0.05f;
    bool firstMouseMovement = true; //used to avoid the sudden camera movement the first time the application receives mouse movement.
    double mousePrevX = 0.0, mousePrevY = 0.0; //previous mouse position
    double deltaMouseX = 0.0, deltaMouseY = 0.0; //change in mouse position from previous callback, not necessarily previous frame.
    double sensitivityX = 0.01, sensitivityY = 0.01; //can be static for now, in the past i've used +/- keys to adjust.

public:
    //Camera(Camera const&) = delete; //to explicitly delete the copy constructor
    //write something to the application's view matrix directly. TODO change this access with an event manager class.
    void Update(float frameTime, ComponentManager* compMan);
    void Update(double posX, double posY);
    void Init(ComponentManager* compMan) {} //does nothing, also not required to be called.
    void AdjustMovementSpeed(float multFactor) { movementSensitivity *= multFactor; }
    static Camera& GetInstance(vec3 pos) {
        static Camera instance(pos);
        return instance;
    }
    mat4 GetView() { return view; }
    const vec3 GetPos() const { return pos; }
    const float GetRadius() const { return 1.0f; }
    // must call CalcPerspective before using GetPerspective
    const mat4 GetPerspective() const { return perspective; }
    void CalcPerspective(int width, int height);
};

