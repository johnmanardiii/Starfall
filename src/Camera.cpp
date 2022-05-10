#include "Camera.h"
#include "ComponentManager.h"

template<typename T>
T exponential_growth(T actual, T goal, float factor, float frametime)
{
    return actual + (goal - actual) * factor * frametime;
}

// returns a value between 0 and 1 mapping value between start range and end_range
template<typename T>
T InverseLerp(T start_range, T end_range, T value)
{
    return clamp<float>((value - start_range) / (end_range - start_range), 0.0, 1.0);
}

void Camera::Update(float frameTime, ComponentManager* compMan)
{
    // look at the player
    vec3 target = compMan->GetPlayer().GetPosition();
    float goalCamDist = glm::mix<float>(highestCamDistZ, loweestCamDistZ,
        fabs(compMan->GetPlayer().GetCurrentSpeed()) / compMan->GetPlayer().GetMaxSpeed());
    currentCamDistZ = exponential_growth(currentCamDistZ, goalCamDist, .035f * 60.0f, frameTime);
    vec3 goal_pos = get_wanted_pos(compMan);
    pos = exponential_growth(pos, goal_pos, .07f * 60.0f, frameTime);
    // check to ensure that the camera keeps up with the player at least 3 units away
    // from its intended position
    if (distance(pos, goal_pos) > max_lerp_distance)
    {
        // move pos in the direction of goal_pos just within range
        pos = goal_pos + normalize(pos - goal_pos) * max_lerp_distance;
    }
    vec3 up = vec3(0, 1, 0);
    mat4 lookAt = glm::lookAt(pos, target, up); //first person camera. "looks at" the direction of target from the starting point of pos.
    // update last view for motion blur
    lastView = view;
    view = lookAt; //something can watch view and do something based on that value.
}

glm::vec3 Camera::get_wanted_pos(ComponentManager* compMan)
{
    vec3 target_pos = compMan->GetPlayer().GetPosition() + currentCamDistZ * -compMan->GetPlayer().GetForward() +
        vec3(0, 1, 0) * camDistHeight;
    return target_pos;
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

void Camera::CalcPerspective(float frametime, int width, int height, ComponentManager* compMan)
{
    // TODO: make this a lerp of some terminal speed made by mitchell when its time
    // change the 15 magic number.
    float goalFov = glm::mix<float>(lowFov, highFov, InverseLerp<float>(0.0f,
        compMan->GetPlayer().GetMaxSpeed(), fabs(compMan->GetPlayer().GetCurrentSpeed())));
    currentFov = exponential_growth(currentFov, goalFov, .035f * 60.0f, frametime);
    // set last perspective for rendering motion blur
    lastPerspective = perspective;
    //currentFov
    perspective = glm::perspective((float)(glm::radians(currentFov)), static_cast<float>(width) / height, 0.1f, 1000.0f);
}