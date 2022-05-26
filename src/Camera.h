#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //for lookat
#include <glm/gtc/type_ptr.hpp> //for value_ptr

#include <cmath>
#include <array>
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
    const float lowestCamDistZ = 11.0f;  // camera is closer when moving at max speed.
    const float highestCamDistZ = 15.0f;    // camera is farther back when still. Lerped between so camera doesn't lag too much with FOV change
    const float backwardsCamDistZ = 20.0f;   // amount camera is behind player when travelling backwards
    float camDistHeight = 3.5f;   // desired height of camera position above player
    const float minCamHeight = 2.0f;
    const float maxCamHeight = 4.3f;
    const float max_lerp_distance = 3.0f;   // max distance before setting camera position manually (to prevent too much cam lag)

    vec3 get_wanted_pos(ComponentManager* compMan);     // gets the desired point behind the player
    void CalcPerspective(float frametime, int width, int height, ComponentManager* compMan);
    void extractVFPlanes();
    std::array<glm::vec4,6> planes; //left, right, bottom, top, near, far
public:
    void Update(float frameTime, int width, int height, ComponentManager* compMan);
    static Camera& GetInstance(vec3 pos) {
        static Camera instance(pos);
        return instance;
    }
    const vec3 GetPos() const { return pos; }
    const mat4 GetView() const { return view; }
    const mat4 GetPerspective() const { return perspective; }
    const mat4 GetPrevView() const { return lastView; }
    const mat4 GetPrevProj() const { return lastPerspective; }
    const std::array<glm::vec4,6>& getVFCPlanes() const { return planes; }
    bool alt_pressed = false;
};

