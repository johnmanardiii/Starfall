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

    UpdatePlayerAnimations(frameTime);
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

quat EulerToQuat(vec3 angles)
{
    return quat(glm::rotate(mat4(1), radians<float>(angles.x), vec3(1, 0, 0))
        * glm::rotate(mat4(1), radians<float>(angles.y), vec3(0, 1, 0))
        * glm::rotate(mat4(1), radians<float>(angles.z), vec3(0, 0, 1)));
}

Player::Player(vec3 pos) : pos (pos)
{
    rArmIdle = EulerToQuat(rightArmIdle);
    lArmIdle = EulerToQuat(leftArmIdle);
    rBoostForwards = EulerToQuat(rightArmBoostForward);
    lBoostForwards = EulerToQuat(leftArmBoostForward);
    rArmTurnOut = EulerToQuat(rightArmTurnOut);
    lArmTurnOut = EulerToQuat(leftArmTurnOut);
    rBoostBackwards = EulerToQuat(rightArmBoostBackward);
    lBoostBackwards = EulerToQuat(leftArmBoostBackward);
}

void Player::Init(ComponentManager* compMan, shared_ptr<EulerTransform> pTrans, 
    shared_ptr<PlayerTransform> head,shared_ptr<PlayerTransform> arm1,
    shared_ptr<PlayerTransform> arm2)
{
    pTransform = pTrans;
    trans = static_pointer_cast<Transform>(pTransform);
    // set child transforms
    headTrans = head;
    arm1Trans = arm1;
    arm2Trans = arm2;

    // set initial positions of each body part
    arm1Trans->SetPos(vec3(-.9, -1.5, 0));
    
    arm2Trans->SetPos(vec3(.9, -1.5, 0));
    headTrans->SetPos(vec3(0, -1.8, 0));
    GameObject obj = compMan->GetGameObject(pName);
    int index = obj.GetComponentLocation("Movement");
    movement = static_pointer_cast<PlayerMovement>(compMan->GetComponent("Movement", index));

}

void Player::SetInput(int index, bool val)
{
    movement->SetInput(index, val);
}

void Player::SetManualRotations()
{
    mat4 manualRightArmPosition = glm::rotate(mat4(1), radians<float>(rightArmEulerOffset.x), vec3(1, 0, 0))
        * glm::rotate(mat4(1), radians<float>(rightArmEulerOffset.y), vec3(0, 1, 0))
        * glm::rotate(mat4(1), radians<float>(rightArmEulerOffset.z), vec3(0, 0, 1));
    arm1Trans->SetBaseRotation(manualRightArmPosition);

    // do the same for arm 2
    mat4 manualLeftArmPosition = glm::rotate(mat4(1), radians<float>(leftArmEulerOffset.x), vec3(1, 0, 0))
        * glm::rotate(mat4(1), radians<float>(leftArmEulerOffset.y), vec3(0, 1, 0))
        * glm::rotate(mat4(1), radians<float>(leftArmEulerOffset.z), vec3(0, 0, 1));
    arm2Trans->SetBaseRotation(manualLeftArmPosition);
}

void Player::SetAutomaticRotations(float frameTime)
{
    int thrust = movement->inputBuffer[W] - movement->inputBuffer[S];
    int rTurn = movement->inputBuffer[A];
    int lTurn = movement->inputBuffer[D];
    quat rGoalRot = rArmIdle;
    quat lGoalRot = lArmIdle;
    bool thrustInfluence = false;
    // calculate where luna wants her arms to go based on input.
    if (thrust > 0)
    {
        rGoalRot = rBoostForwards;
        lGoalRot = lBoostForwards;
        thrustInfluence = true;
    }
    else if (thrust < 0)
    {
        rGoalRot = rBoostBackwards;
        lGoalRot = lBoostBackwards;
        thrustInfluence = true;
    }

    // handle sideways movement
    if (rTurn)
    {
        if (thrustInfluence)
        {
            // blend with existing goal
            rGoalRot = slerp(rGoalRot, rArmTurnOut, .8f);
        }
        else
        {
            rGoalRot = rArmTurnOut;
        }
    }
    if (lTurn)
    {
        if (thrustInfluence)
        {
            // blend with existing goal
            lGoalRot = slerp(lGoalRot, lArmTurnOut, .8f);
        }
        else
        {
            lGoalRot = lArmTurnOut;
        }
    }
    rRot = slerp(rRot, rGoalRot, .2f * 60.0f * frameTime);
    lRot = slerp(lRot, lGoalRot, .2f * 60.0f * frameTime);
    arm1Trans->SetBaseRotation(mat4(rRot));
    arm2Trans->SetBaseRotation(mat4(lRot));
}

void Player::AnimatePlayerModel(float frameTime)
{
    // if LUNA is still, have them float a bit above the ground. (gets reset upon move)
    // TODO: Fix idle float calculations
    //AddIdleOffset(frameTime);
    SetAutomaticRotations(frameTime);
    //SetManualRotations();
    
    
    // lerp LUNA to the rotation they are accelerating in (around their local z axis)
    float goalZRotation = -movement->GetAngularSpeed() * 15.0f;
    currentZRotation = exponential_growth(currentZRotation, goalZRotation, .02 * 60.0f, frameTime);
    pTransform->SetRoll(currentZRotation);

    int thrust = movement->inputBuffer[W] - movement->inputBuffer[S];
    float goalForwardsRotation = thrust * 30.0f;
    currentXRotation = exponential_growth(currentXRotation, goalForwardsRotation, .2 * 60.0f, frameTime);
    pTransform->SetLean(currentXRotation);
}

// animation code begins here:
void Player::UpdatePlayerAnimations(float frameTime)
{
    // update the values on Imgui
    if (ImGui::CollapsingHeader("PlayerAnims")) {}
        // Gradient
        ImGui::DragFloat3("RightArmEulerOffset", (float*)&rightArmEulerOffset);
        ImGui::DragFloat3("LeftArmEulerOffset", (float*)&leftArmEulerOffset);

        if (ImGui::Button("Set Right Arm Boost Forward"))
        {
            rRot = rBoostForwards;
        }
        if (ImGui::Button("Set Left Arm Boost Forward"))
        {
            lRot = lBoostForwards;
        }

        if (ImGui::Button("Set Right Arm Turn Out"))
        {
            rRot = rArmTurnOut;
        }
        if (ImGui::Button("Set Left Arm Turn Out"))
        {
            lRot = lArmTurnOut;
        }

        if (ImGui::Button("Set Right Arm Boost Backward"))
        {
            rRot = rBoostBackwards;
        }
        if (ImGui::Button("Set Left Arm Boost Backward"))
        {
            lRot = lBoostBackwards;
        }
    // }

    // Animate player model based on input
    AnimatePlayerModel(frameTime);
}