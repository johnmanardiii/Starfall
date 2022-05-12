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

// The Camera class is a specific type of camera that follows the player and looks at the player.
// It has springing (smoothing towards desired position), different FOV's based on speed,
// and a different following distance based on speed. The following distance is interpolated
// with the FOV to give a feeling of speed.
class Camera
{
private:
    Camera(vec3 pos) { pos = pos; }
    vec3 pos = vec3(0);
    mat4 view = mat4(1.0f);
    mat4 lastView = mat4(1.0f);
    mat4 perspective = mat4(1.0f);
    mat4 lastPerspective = mat4(1.0f);
    const float lowFov = 80.0f; // FOV used when still
    const float highFov = 100.0f;   // FOV used at max speed
    float currentFov = 70.0f;   // FOV used in camera
    float currentCamDistZ = 12.0f;  // current Camera distance on Z that gets interpolated w/ speed
    const float lowestCamDistZ = 7.0f;  // camera is closer when moving at max speed.
    const float highestCamDistZ = 12.0f;    // camera is farther back when still. Lerped between so camera doesn't lag too much with FOV change
    const float backwardsCamDistZ = 8.0f;   // amount camera is behind player when travelling backwards
    const float camDistHeight = 4.6f;   // desired height of camera position above player
    const float max_lerp_distance = 3.0f;   // max distance before setting camera position manually (to prevent too much cam lag)

    vec3 get_wanted_pos(ComponentManager* compMan);     // gets the desired point behind the player
    void CalcPerspective(float frametime, int width, int height, ComponentManager* compMan);

public:
    void Update(float frameTime, int width, int height, ComponentManager* compMan);
    static Camera& GetInstance(vec3 pos) {
        static Camera instance(pos);
        return instance;
    }
    const vec3 GetPos() const { return pos; }
    const mat4 const GetView() { return view; }
    const mat4 GetPerspective() const { return perspective; }
    const mat4 const GetPrevView() { return lastView; }
    const mat4 const GetPrevProj() { return lastPerspective; }
};

