#include "PlayerMovement.h"
#include "Movement.h"

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
    float speedInput = inputBuffer[0] - inputBuffer[2];

    // TODO: add acceleration to input, and make luna lean forward and backward
    // based on acceleration rather than direct speed.
    accel += speedInput * speed;
    velocity += accel;
    // Apply Gravity
    velocity.y -= (inputBuffer[4] ? STRONG_GRAVITY_MULT : GRAVITY_MULT) * frameTime;

    // Multiply velocity value by factor determined by slope difference
    //   up to double speed if sloped down, down to zeroing speed if sloped up
    static float twoOverPi = 2.0f / pi<float>();
    float slopeDiff = GetSlopeDiff(frameTime);
    float slopeMult = -atan(slopeDiff / 4.0f) * twoOverPi * MAX_SLOPE_MULT + 1.0f;
    slopeMult = inputBuffer[4] ? slopeMult : 1.0f;
    velocity *= slopeMult;

    // move player by their velocity
    vec3 translation = velocity * frameTime;
    trans->ApplyTranslation(translation);
    // Perform ground collision resolution 
    //   (make sure player doesn't fall through floor)
    GroundCollision();

    currentSpeed = glm::length(velocity);
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

float PlayerMovement::GetSlopeDiff(float frameTime)
{
    vec3 forward = velocity * frameTime;
    vec3 pos = trans->GetPos();
    float currentH = pos.y,
        nextH = max(pos.y - (inputBuffer[4] ? STRONG_GRAVITY_MULT : GRAVITY_MULT) * frameTime,
            heightCalc(pos.x + forward.x, pos.z + forward.z));
    return nextH - currentH;
}

void PlayerMovement::GroundCollision()
{
    vec3 pos = trans->GetPos();
    pos.y = max(pos.y, heightCalc(pos.x, pos.z));
    trans->SetPos(pos);
}

float PlayerMovement::GetAngularSpeed()
{
    return currentAngularSpeed;
}

float PlayerMovement::GetSpeed()
{
    return currentSpeed;
}