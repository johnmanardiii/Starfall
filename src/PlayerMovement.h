#pragma once
#include "Movement.h"

enum Input {W = 0, A = 1, S = 2, D = 3, LSHIFT = 4};

#define MAX_SPEED 50.0f
#define SPEED 3.0f
#define GRAVITY_MULT 2.0f
#define STRONG_GRAVITY_MULT 3.0f
#define SPEED_FALLOFF 4.5f

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
private:
	void ApplyGravity(float frameTime);
	float speed = 3.0f;	// speed in units/second
	float rotationSpeed = 90.0f; // 90 degrees per second
	float currentAngularSpeed = 0.0f;
	float currentSpeed = 0.0f;
	float currentDownSpeed = 0.0f;
	bool inputBuffer[5] = { false, false, false, false, false };

};