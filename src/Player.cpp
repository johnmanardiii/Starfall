#include "Player.h"
#include "ComponentManager.h"
#include <glm/gtx/quaternion.hpp>
#include <iostream>
using namespace std;

void Player::Update(float frameTime, ComponentManager* compMan)
{
    // store result of W and S as speed
    float speedInput = inputBuffer[0] - inputBuffer[2];
    // store result of A and D as intended rotation
    float inputRotation = inputBuffer[3] - inputBuffer[1];

    // rotate the player's orientation around the y axis by the amount specified
    mat4 newRotation = glm::rotate(mat4(1), -inputRotation * glm::radians(rotationSpeed * frameTime), vec3(0, 1, 0)) *
        mat4(trans->GetRot());
    trans->SetRot(quat(newRotation));

    // move player in the forward direction based on input
    pos += trans->GetForward() * speedInput * speed * frameTime;
    // snap player to ground
    SetPosToGround();
    trans->SetPos(pos);
}

vec3 Player::GetForward()
{
    return trans->GetForward();
}

void Player::SetPosToGround()
{
    // hardcoded value from sin function that generates the terrain.
    pos.y = sin(pos.x / 10.0f) * sin(pos.z / 10.0f) * 5 - playerYOffset;
}

Player::Player(vec3 pos) : pos (pos)
{

}

void Player::Init(ComponentManager* compMan)
{
    GameObject obj = compMan->GetGameObject(pName);
    size_t index = obj.GetComponentLocation("Transform");
    trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));
}