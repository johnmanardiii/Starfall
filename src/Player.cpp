#include "Player.h"
#include "ComponentManager.h"
#include "HeightCalc.h"
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <math.h>
using namespace std;

template<typename T>
T exponential_growth(T actual, T goal, float factor, float frametime)
{
    return actual + (goal - actual) * factor * frametime;
}

void Player::AnimatePlayerModel(float frameTime)
{
    // if LUNA is still, have them float a bit above the ground. (gets reset upon move)
    // TODO: Fix idle float calculations
    //AddIdleOffset(frameTime);

    // lerp LUNA to the rotation they are accelerating in (around their local z axis)
    float goalZRotation = -movement->GetAngularSpeed() * 15.0f;
    currentZRotation = exponential_growth(currentZRotation, goalZRotation, .02 * 60.0f, frameTime);
    pTransform->SetRoll(currentZRotation);

    float goalForwardsRotation = movement->GetSpeed() > 0 ? 40.0f : 0.0f;   // TODO add this to player class as const
    currentXRotation = exponential_growth(currentXRotation, goalForwardsRotation, .02 * 60.0f, frameTime);
    pTransform->SetLean(currentXRotation);
}

void Player::Update(float frameTime, ComponentManager* compMan)
{
    // add to idle time if idle
    if (fabs(movement->GetSpeed()) < 0.001f)
    {
        idleTime += frameTime;
    }
    else
    {
        idleTime = 0;
    }

    // Animate player model based on input
    AnimatePlayerModel(frameTime);
}

vec3 Player::GetForward()
{
    return pTransform->GetForward();
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

Player::Player(vec3 pos) : pos (pos)
{

}

void Player::Init(ComponentManager* compMan, shared_ptr<EulerTransform> pTrans, 
    shared_ptr<Transform> head,shared_ptr<Transform> arm1,
    shared_ptr<Transform> arm2)
{

    pTransform = pTrans;
    trans = static_pointer_cast<Transform>(pTransform);
    // set child transforms
    headTrans = head;
    arm1Trans = arm1;
    arm2Trans = arm2;

    // set initial positions of each body part
    arm1Trans->SetPos(vec3(-1.2, 0, 0));
    
    arm2Trans->SetPos(vec3(1.2, 0, 0));
    headTrans->SetPos(vec3(0, -2.0, 0));
    GameObject obj = compMan->GetGameObject(pName);
    int index = obj.GetComponentLocation("Movement");
    movement = static_pointer_cast<PlayerMovement>(compMan->GetComponent("Movement", index));

}

void Player::SetInput(int index, bool val)
{
    movement->SetInput(index, val);
}