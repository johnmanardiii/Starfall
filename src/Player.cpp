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
    UpdatePlayerAnimations(frameTime);
}

vec3 Player::GetForward()
{
    return pTransform->GetForward();
}

vec3 Player::GetVelocity()
{
    return normalize(movement->GetVel());
}

// Converts and Euler angle to a quaternion
quat EulerToQuat(vec3 angles)
{
    return quat(glm::rotate(mat4(1), radians<float>(angles.x), vec3(1, 0, 0))
        * glm::rotate(mat4(1), radians<float>(angles.y), vec3(0, 1, 0))
        * glm::rotate(mat4(1), radians<float>(angles.z), vec3(0, 0, 1)));
}

// Converts all stored Euler rotations to quaternions for slerping
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

    hTurnRight = EulerToQuat(headTurnRight);
    hTurnLeft = EulerToQuat(headTurnLeft);
}

void Player::Init(ComponentManager* compMan, shared_ptr<EulerTransform> pTrans, 
    shared_ptr<PlayerTransform> head,shared_ptr<PlayerTransform> arm1,
    shared_ptr<PlayerTransform> arm2, shared_ptr<HeadRenderer> headr)
{
    pTransform = pTrans;
    headRenderer = headr;
    trans = static_pointer_cast<Transform>(pTransform);
    // set child transforms
    headTrans = head;
    arm1Trans = arm1;
    arm2Trans = arm2;

    // set initial positions of each body part
    arm1Trans->SetPos(vec3(-1.0, -1.5, 0));
    
    arm2Trans->SetPos(vec3(1.0, -1.5, 0));
    headTrans->SetPos(vec3(0, -1.8, 0));
    GameObject obj = compMan->GetGameObject(pName);
    int index = obj.GetComponentLocation("Movement");
    movement = static_pointer_cast<PlayerMovement>(compMan->GetComponent("Movement", index));
}

void Player::SetInput(int index, bool val)
{
    movement->SetInput(index, val);
}


// Set each orientation to that of its ImGui values
void Player::SetManualRotations()
{
    mat4 manualRightArmRotation = glm::rotate(mat4(1), radians<float>(rightArmEulerOffset.x), vec3(1, 0, 0))
        * glm::rotate(mat4(1), radians<float>(rightArmEulerOffset.y), vec3(0, 1, 0))
        * glm::rotate(mat4(1), radians<float>(rightArmEulerOffset.z), vec3(0, 0, 1));
    arm1Trans->SetBaseRotation(manualRightArmRotation);

    // do the same for arm 2
    mat4 manualLeftArmRotation = glm::rotate(mat4(1), radians<float>(leftArmEulerOffset.x), vec3(1, 0, 0))
        * glm::rotate(mat4(1), radians<float>(leftArmEulerOffset.y), vec3(0, 1, 0))
        * glm::rotate(mat4(1), radians<float>(leftArmEulerOffset.z), vec3(0, 0, 1));
    arm2Trans->SetBaseRotation(manualLeftArmRotation);

    mat4 manualHeadRotation = glm::rotate(mat4(1), radians<float>(headEulerOffset.x), vec3(1, 0, 0))
        * glm::rotate(mat4(1), radians<float>(headEulerOffset.y), vec3(0, 1, 0))
        * glm::rotate(mat4(1), radians<float>(headEulerOffset.z), vec3(0, 0, 1));
    headTrans->SetBaseRotation(manualHeadRotation);
}

// Pushes each arm out towards its target location, smoothed
void Player::UpdateArmAnimations(float frameTime, int thrust, int rTurn, int lTurn)
{
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
    rRot = slerp(rRot, rGoalRot, .16f * 60.0f * frameTime);
    lRot = slerp(lRot, lGoalRot, .16f * 60.0f * frameTime);
    arm1Trans->SetBaseRotation(mat4(rRot));
    arm2Trans->SetBaseRotation(mat4(lRot));
}

// Makes the player look ahead to the direction they are turning
void Player::UpdateHeadAnimations(float frameTime, int headTurn)
{
    quat headGoal = glm::identity<quat>();
    if (headTurn > 0)
    {
        // turning right
        headGoal = hTurnRight;
    }
    else if (headTurn < 0)
    {
        // turning left
        headGoal = hTurnLeft;
    }
    hRot = slerp(hRot, headGoal, .10f * 60.0f * frameTime);
    headTrans->SetBaseRotation(mat4(hRot));
}

// Sets the lean and roll on the player based on input, smoothed
void Player::SetBaseRotation(float frameTime, int thrust)
{
    float goalZRotation = -movement->GetAngularSpeed() * 15.0f;
    currentZRotation = exponential_growth(currentZRotation, goalZRotation, .02 * 60.0f, frameTime);
    pTransform->SetRoll(currentZRotation);

    float goalForwardsRotation = thrust * 30.0f;
    if (thrust < 0)
    {
        goalForwardsRotation *= 0;
    }
    currentXRotation = exponential_growth(currentXRotation, goalForwardsRotation, .2 * 60.0f, frameTime);
    pTransform->SetLean(currentXRotation);
}

// Animates the player only visually to slightly hover up and down while idle
void Player::SetHoverAnimation(float frameTime, int thrust)
{
    float goalHoverOffset = 0.0;
    if (thrust == 0)
    {
        idleTime += frameTime;
        goalHoverOffset = sin(idleTime * .4) * idleHeight;
    }
    else
    {
        idleTime = 0;
    }
    currentFloatOffset = exponential_growth(currentFloatOffset, goalHoverOffset, .2 * 60.0f, frameTime);
    pTransform->SetVisualOffset(currentFloatOffset);
}

void Player::SetAutomaticRotations(float frameTime)
{
    int thrust = movement->inputBuffer[W] - movement->inputBuffer[S];
    int rTurn = movement->inputBuffer[A];
    int lTurn = movement->inputBuffer[D];

    UpdateArmAnimations(frameTime, thrust, rTurn, lTurn);
    
    // turn the head based on the desired input
    int head_turn = rTurn - lTurn;
    UpdateHeadAnimations(frameTime, head_turn);

    // Animate pitch and roll based on turning / speed
    SetBaseRotation(frameTime, thrust);

    // Animate bobbing when idle    // TODO: AFTER CODE REVIEW, FIX THIS 
    SetHoverAnimation(frameTime, thrust);
}

void Player::AnimatePlayerModel(float frameTime)
{
    // if LUNA is still, have them float a bit above the ground. (gets reset upon move)
    // TODO: Fix idle float calculations -- maybe I will add this back in if time
    //AddIdleOffset(frameTime);
    SetAutomaticRotations(frameTime);
    // SetManualRotations();
}

// animation code begins here:
void Player::UpdatePlayerAnimations(float frameTime)
{
    // update the values on Imgui
    if (ImGui::CollapsingHeader("PlayerAnims")) {}
        // Gradient
        ImGui::DragFloat3("RightArmEulerOffset", (float*)&rightArmEulerOffset);
        ImGui::DragFloat3("LeftArmEulerOffset", (float*)&leftArmEulerOffset);
        ImGui::DragFloat3("HeadEulerOffset", (float*)&headEulerOffset);

        ImGui::DragFloat2("Eye Position", (float*)&headRenderer->eye1Pos);
        ImGui::DragFloat("Eye Radius", (float*)&headRenderer->eye1Radius);
        ImGui::DragFloat("Eye open pct", (float*)&headRenderer->eyeOpenPct);

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