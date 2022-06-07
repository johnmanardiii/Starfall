#include "Camera.h"
#include "ComponentManager.h"
#include "HeightCalc.h"

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
    int currentPlayerSpeed = compMan->GetPlayer().movement->inputBuffer[W] - 
        compMan->GetPlayer().movement->inputBuffer[S];
    float goalCamDist;
    float goalCamHeight;
    if (currentPlayerSpeed >= 0)
    {
        goalCamDist = glm::mix<float>(lowestCamDistZ, highestCamDistZ,
            (float)currentPlayerSpeed);
        goalCamHeight = glm::mix<float>(minCamHeight, maxCamHeight,
            (float)currentPlayerSpeed);
    }
    else
    {
        // don't have the camera try to go so far back 
        // because it looks really bad when it lags behind the player like this
        goalCamDist = backwardsCamDistZ;
        goalCamHeight = maxCamHeight;
    }
    currentCamDistZ = exponential_growth(currentCamDistZ, goalCamDist, .035f * 60.0f, frameTime);
    camDistHeight = exponential_growth(camDistHeight, goalCamHeight, .35f * 60.0f, frameTime);
    vec3 goal_pos = get_wanted_pos(compMan);
    pos = exponential_growth(pos, goal_pos, .07f * 60.0f, frameTime);
    // check to ensure that the camera keeps up with the player at least 3 units away
    // from its intended position
    if (distance(pos, goal_pos) > max_lerp_distance)
    {
        // move pos in the direction of goal_pos just within range
        pos = goal_pos + normalize(pos - goal_pos) * max_lerp_distance;
    }
    float currHeight = std::max<float>(pos.y, HeightCalc::heightCalc(pos.x, pos.z) + 1.0f);
    pos = vec3(pos.x, currHeight, pos.z);
    vec3 up = vec3(0, 1, 0);
    mat4 lookAt = glm::lookAt(pos, target, up); //first person camera. "looks at" the direction of target from the starting point of pos.
    // update last view for motion blur
    lastView = view;
    view = lookAt; //something can watch view and do something based on that value.
    extractVFPlanes(); //extract the view frustum planes for the current view and perspective matrices.
}

// Returns a point to interpolate to. Currently returns a point behind the player and above the player.
glm::vec3 Camera::get_wanted_pos(ComponentManager* compMan)
{
    vec3 target_pos;
    float camDistZ = currentCamDistZ;
    if (alt_pressed)
    {
        camDistZ *= -1;
    }
    if (x_pressed)
    {
        target_pos = compMan->GetPlayer().GetPosition() + camDistZ * -compMan->GetPlayer().GetForward() * .3f +
            vec3(0, 1, 0) * camDistHeight * .3f;
    }
    else
    {
        target_pos = compMan->GetPlayer().GetPosition() + camDistZ * -compMan->GetPlayer().GetForward() +
            vec3(0, 1, 0) * camDistHeight;
    }
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

void Camera::extractVFPlanes()
{
    mat4 comp = perspective * view;
    vec3 normal;
    float normalLength;
    //left
    planes[0].x = comp[0][3] + comp[0][0];
    planes[0].y = comp[1][3] + comp[1][0];
    planes[0].z = comp[2][3] + comp[2][0];
    planes[0].w = comp[3][3] + comp[3][0];

    //right
    planes[1].x = comp[0][3] - comp[0][0];
    planes[1].y = comp[1][3] - comp[1][0];
    planes[1].z = comp[2][3] - comp[2][0];
    planes[1].w = comp[3][3] - comp[3][0];

    //bottom
    planes[2].x = comp[0][3] + comp[0][1];
    planes[2].y = comp[1][3] + comp[1][1];
    planes[2].z = comp[2][3] + comp[2][1];
    planes[2].w = comp[3][3] + comp[3][1];

    //top
    planes[3].x = comp[0][3] - comp[0][1];
    planes[3].y = comp[1][3] - comp[1][1];
    planes[3].z = comp[2][3] - comp[2][1];
    planes[3].w = comp[3][3] - comp[3][1];

    //near
    planes[4].x = comp[0][2];
    planes[4].y = comp[1][2];
    planes[4].z = comp[2][2];
    planes[4].w = comp[3][2];

    //far
    planes[5].x = comp[0][3] - comp[0][2];
    planes[5].y = comp[1][3] - comp[1][2];
    planes[5].z = comp[2][3] - comp[2][2];
    planes[5].w = comp[3][3] - comp[3][2];

    //normalize
    for (vec4& plane : planes) {
        normal = vec3(plane.x, plane.y, plane.z);
        normalLength = length(normal);
        float w = plane.w;
        plane = vec4(normal / normalLength, w / normalLength);
    }
}
