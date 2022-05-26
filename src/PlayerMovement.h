#pragma once
#include "Movement.h"
#include <algorithm>

enum Input {W = 0, A = 1, S = 2, D = 3, LSHIFT = 4};

#define SPEED_MULT 60.0f

#define MAX_SPEED 50.0f * SPEED_MULT
#define SPEED 1.0f * SPEED_MULT
#define GRAVITY_MULT 0.75f * 60.0f
#define STRONG_GRAVITY_MULT 1.0f * 60.0f
#define SPEED_FALLOFF 2.5f

class PlayerMovement : public Movement
{
public:
	PlayerMovement(std::string name);
	PlayerMovement(std::string name, vec3 vel);
	void SetInput(int index, bool value);
	void Update(float frameTime, ComponentManager* compMan);
	void UpdateRotation(float frameTime);
	void GroundCollision(float frameTime);
	float GetSlopeDiff(float frameTime);
	float GetSpeed();
	float GetMaxSpeed();
	float GetAngularSpeed();
	bool inputBuffer[5] = { false, false, false, false, false };
private:
	void ApplyGravity(float frameTime);
	float speed = 3.0f;	// speed in units/second
	float rotationSpeed = 90.0f; // 90 degrees per second
	float currentAngularSpeed = 0.0f;
	float currentSpeed = 0.0f;
	float currentDownSpeed = 0.0f;
};
