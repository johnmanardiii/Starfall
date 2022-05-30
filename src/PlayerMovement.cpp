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
    // Rotate Luna first so forward direction is correct
    UpdateRotation(frameTime);

    // store result of W and S as speed
    float speedInput = inputBuffer[W] - inputBuffer[S];

    // TODO: add acceleration to input, and make luna lean forward and backward
    // based on acceleration rather than direct speed.

    // Apply velocity in the direction Luna is facing
    vec3 dir = glm::rotate(trans->GetRot(), vec3(0, 0, -1));
    velocity +=  dir * speedInput * SPEED * frameTime;

    // Apply downward velocity
    //   (gravity is accelleration, 
    //   but its effect is adding downward velocity each frame)
    ApplyGravity(frameTime);

    // If holding left shift add more downward veloctity (to gain extra speed)
    if (inputBuffer[LSHIFT])
    {
        velocity.y -= STRONG_GRAVITY_MULT * frameTime;
    }

    // Sloped movement resolution
    //   depends on whether shift is held or not
    GroundCollision(frameTime);

    // Actually move Luna
    trans->ApplyTranslation(velocity * frameTime);

    // Reset Luna's position to the ground height
    //   if they're below the ground
    vec3 pos = trans->GetPos();
    pos.y = (glm::max)(pos.y, HeightCalc::heightCalc(pos.x, pos.z));
    trans->SetPos(pos);

    // Lose a small amount of velocity (friction)
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
        nextH = (std::max)(pos.y - (inputBuffer[LSHIFT] ? STRONG_GRAVITY_MULT : GRAVITY_MULT) * frameTime,
            HeightCalc::heightCalc(pos.x + forward.x, pos.z + forward.z));
    return nextH - currentH;
}

void PlayerMovement::GroundCollision(float frameTime)
{
    // Get Luna's position and a point in the direction of Luna's velocity
    vec3 pos = trans->GetPos(),
        offPos = pos + velocity * frameTime;
    // Set the offset point on the ground if it was below the ground
    offPos.y = (glm::max)(offPos.y, HeightCalc::heightCalc(offPos.x, offPos.z));

    // This is the slope vector. If the offset point was above the ground,
    //   this will just be in the same direction as Luna's velocity.
    //   If the offset point was below the ground, this vector will point
    //   in the direction of the slope of the gound.
    vec3 slopeVec = offPos - pos;

    // Return if Luna is not moving
    if (glm::length(slopeVec) < 0.001)
    {
        return;
    }
    slopeVec = glm::normalize(slopeVec);

    // If left shift is held, project the velocity
    //   onto the normalized slope vector
    //   (This will redirect any extra downward velocity
    //   in the direction of the slope).
    if (inputBuffer[LSHIFT])
    {
        // project velocity onto slope vector
        velocity = glm::dot(velocity, slopeVec) * slopeVec;
    }
    // If left shit not held, redirect all velocity to the
    //   direction of the slope
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
    return length(velocity);
}

float PlayerMovement::GetMaxSpeed()
{
    return speed;
}
