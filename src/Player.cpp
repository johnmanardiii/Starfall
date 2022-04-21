#include "Player.h"
#include "ComponentManager.h"
#include <glm/gtx/quaternion.hpp>
#include <iostream>
using namespace std;

template<typename T>
T exponential_growth(T actual, T goal, float factor, float frametime)
{
    return actual + (goal - actual) * factor * frametime;
}

void Player::AnimatePlayerModel(float frameTime)
{
    // if LUNA is still, have them float a bit above the ground. (gets reset upon move)
    AddIdleOffset(frameTime);

    // lerp LUNA to the rotation they are accelerating in (around their local z axis)
    float goalZRotation = -currentRotationChange * 15.0f;
    currentZRotation = exponential_growth(currentZRotation, goalZRotation, .02 * 60.0f, frameTime);
    pTransform->SetRoll(currentZRotation);
}

void Player::Update(float frameTime, ComponentManager* compMan)
{
    // store result of W and S as speed
    float speedInput = inputBuffer[0] - inputBuffer[2];
    // store result of A and D as intended rotation
    float inputRotation = inputBuffer[3] - inputBuffer[1];

    currentSpeed = speedInput * speed;
    currentRotationChange = -inputRotation * glm::radians(rotationSpeed);

    // rotate the player's orientation around the y axis by the amount specified
    mat4 newRotation = glm::rotate(mat4(1), currentRotationChange * frameTime, vec3(0, 1, 0)) *
        mat4(trans->GetRot());
    trans->SetRot(quat(newRotation));

    // move player in the forward direction based on input
    pos += trans->GetForward() * currentSpeed * frameTime;
    // snap player to ground
    SetPosToGround();

    // add to idle time if idle
    if (speedInput == 0.0f)
    {
        idleTime += frameTime;
    }
    else
    {
        idleTime = 0;
    }

    // Animate player model based on input
    AnimatePlayerModel(frameTime);

    // Set player model to final pos
    trans->SetPos(pos);
}

vec3 Player::GetForward()
{
    return trans->GetForward();
}

void Player::AddIdleOffset(float frameTime)
{
    float goalOffset = 0.0f;
    if (idleTime != 0.0f)
    {
        // try to lerp floatoffset back to 0.
        goalOffset = sin(idleTime * 1.5f) * .2f;
    }
    currentFloatOffset = exponential_growth(currentFloatOffset,
        goalOffset, .05f * 60.0f, frameTime);
    pos.y += currentFloatOffset;
}

void Player::SetPosToGround()
{
    // hardcoded value from sin function that generates the terrain.
    pos.y = sin(pos.x / 10.0f) * sin(pos.z / 10.0f) * 5 + playerYOffset;
}

Player::Player(vec3 pos) : pos (pos)
{

}

void Player::Init(ComponentManager* compMan, shared_ptr<RollTransform> pTrans, 
    shared_ptr<Transform> head,shared_ptr<Transform> arm1,
    shared_ptr<Transform> arm2)
{
    GameObject obj = compMan->GetGameObject(pName);
    size_t index = obj.GetComponentLocation("Transform");
    trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));

    pTransform = pTrans;
    // set child transforms
    headTrans = head;
    arm1Trans = arm1;
    arm2Trans = arm2;

    // set initial positions of each body part
    arm1Trans->SetPos(vec3(-1.2, 0, 0));
    
    arm2Trans->SetPos(vec3(1.2, 0, 0));
    headTrans->SetPos(vec3(0, -2.0, 0));

}