#include "PlayerMovement.h"
#include "ComponentManager.h"

PlayerMovement::PlayerMovement(std::string name) : Movement(name) {}
PlayerMovement::PlayerMovement(std::string name, vec3 vel) : Movement(name, vel) {}


void PlayerMovement::SetInput(int index, bool value)
{
	inputBuffer[index] = value;
}

void PlayerMovement::Update(float frameTime, ComponentManager* compMan)
{
    UpdateRotation(frameTime);

    // store result of W and S as speed
    float speedInput = inputBuffer[W] - inputBuffer[S];

    // TODO: add acceleration to input, and make luna lean forward and backward
    // based on acceleration rather than direct speed.
    vec3 dir = glm::rotate(trans->GetRot(), vec3(0, 0, -1));
    velocity +=  dir * speedInput * SPEED * frameTime;
    cout << "Speed: " << glm::length(velocity) << endl;
    ApplyGravity(frameTime);
    if (inputBuffer[LSHIFT])
    {
        velocity.y -= STRONG_GRAVITY_MULT * frameTime;
    }
    GroundCollision(frameTime);
    trans->ApplyTranslation(velocity);
    vec3 pos = trans->GetPos();
    pos.y = (glm::max)(pos.y, heightCalc(pos.x, pos.z));
    trans->SetPos(pos);
    velocity -= velocity * SPEED_FALLOFF * frameTime;
}

void PlayerMovement::UpdateRotation(float frameTime)
{

    // store result of A and D as intended rotation
    float inputRotation = inputBuffer[3] - inputBuffer[1];
    currentAngularSpeed = -inputRotation * glm::radians(rotationSpeed);

    // rotate the player's orientation around the y axis by the amount specified
    mat4 newRotation = glm::rotate(mat4(1), currentAngularSpeed * frameTime, vec3(0, 1, 0)) *
        mat4(trans->GetRot());
    trans->SetRot(quat(newRotation));
}

void PlayerMovement::ApplyGravity(float frameTime)
{
    velocity.y -= GRAVITY_MULT * frameTime;
}

float PlayerMovement::GetSlopeDiff(float frameTime)
{
    vec3 forward = glm::rotate(trans->GetRot(), vec3(0, 0, -1)) * frameTime;
    vec3 pos = trans->GetPos();
    float currentH = pos.y,
        nextH = max(pos.y - (inputBuffer[4] ? STRONG_GRAVITY_MULT : GRAVITY_MULT) * frameTime,
            heightCalc(pos.x + forward.x, pos.z + forward.z));
    return nextH - currentH;
}

void PlayerMovement::GroundCollision(float frameTime)
{
    vec3 pos = trans->GetPos(),
        offPos = pos + velocity * frameTime;
    offPos.y = (glm::max)(offPos.y, heightCalc(offPos.x, offPos.z));
    vec3 slopeVec = offPos - pos;
    if (glm::length(slopeVec) < 0.001)
    {
        return;
    }
    slopeVec = glm::normalize(slopeVec);
    if (inputBuffer[LSHIFT])
    {
        // project velocity onto slope vector
        velocity = glm::dot(velocity, slopeVec) * slopeVec;
    }
    else
    {
        velocity = slopeVec * glm::length(velocity);
    }

}

float PlayerMovement::GetAngularSpeed()
{
    return currentAngularSpeed;
}

float PlayerMovement::GetSpeed()
{
    return currentSpeed;
}

float PlayerMovement::GetMaxSpeed()
{
    return speed;
}