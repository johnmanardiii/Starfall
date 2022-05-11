#include "Camera.h"
#include "ComponentManager.h"

// interpolates between two values using an exponential falloff
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

// Interpolates camera position towards goal position and updates perspective and view matrices,
// as well as previous perspective and view matrices.
void Camera::Update(float frameTime, int width, int height, ComponentManager* compMan)
{
    CalcPerspective(frameTime, width, height, compMan);
    // look at the player
    vec3 target = compMan->GetPlayer().GetPosition();
    float currentPlayerSpeed = compMan->GetPlayer().GetCurrentSpeed();
    float goalCamDist;
    if (currentPlayerSpeed >= 0)
    {
        goalCamDist = glm::mix<float>(highestCamDistZ, lowestCamDistZ,
            currentPlayerSpeed / compMan->GetPlayer().GetMaxSpeed());
    }
    else
    {
        // don't have the camera try to go so far back 
        // because it looks really bad when it lags behind the player like this
        goalCamDist = backwardsCamDistZ;
    }
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

// Returns a point to interpolate to. Currently returns a point behind the player and above the player.
glm::vec3 Camera::get_wanted_pos(ComponentManager* compMan)
{
    vec3 target_pos = compMan->GetPlayer().GetPosition() + currentCamDistZ * -compMan->GetPlayer().GetForward() +
        vec3(0, 1, 0) * camDistHeight;
    return target_pos;
}

void Camera::CalcPerspective(float frametime, int width, int height, ComponentManager* compMan)
{
    // TODO: make this a lerp of some terminal speed made by mitchell he is done with his work
    float goalFov = glm::mix<float>(lowFov, highFov, InverseLerp<float>(0.0f,
        compMan->GetPlayer().GetMaxSpeed(), fabs(compMan->GetPlayer().GetCurrentSpeed())));
    currentFov = exponential_growth(currentFov, goalFov, .035f * 60.0f, frametime);
    // set last perspective for rendering motion blur
    lastPerspective = perspective;
    //currentFov
    perspective = glm::perspective((float)(glm::radians(currentFov)), static_cast<float>(width) / height, 0.1f, 1000.0f);
}