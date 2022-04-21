#include "Player.h"
#include "ComponentManager.h"
#include <iostream>
using namespace std;

void Player::Update(float frameTime, ComponentManager* compMan)
{
    ProcessWASDInput();
    trans->SetPos(pos);
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

void Player::ProcessWASDInput() {
    if (IsWASDPressed[0]) {
        pos += movementSensitivity * vec3(0, 0.0f, 1);
    }
    if (IsWASDPressed[1]) {
        pos += movementSensitivity * vec3(-1, 0.0f, 0);
    }
    if (IsWASDPressed[2]) {
        pos += movementSensitivity * vec3(0, 0.0f, -1);
    }
    if (IsWASDPressed[3]) {
        pos += movementSensitivity * vec3(1, 0.0f, 0);
    }
}