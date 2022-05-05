#pragma once
class Movement; //#include "Movement.h"
#include "ComponentManager.h"
enum Input {W = 0, A = 1, S = 2, D = 3, LSHIFT = 4};

class PlayerMovement : public Movement
{
public:
	PlayerMovement(std::string name);
	PlayerMovement(std::string name, vec3 vel);
	void SetInput(int index, bool value);
	void Update(float frameTime, ComponentManager* compMan);
	void UpdateRotation(float frameTime);
	void GroundCollision();
	float GetSlopeDiff(float frameTime);
	float GetSpeed();
	float GetAngularSpeed();
private:
	float speed = 15.0f;	// speed in units/second
	float rotationSpeed = 90.0f; // 90 degrees per second
	float currentAngularSpeed = 0.0f;
	float currentSpeed = 0.0f;
	bool inputBuffer[5] = { false, false, false, false, false };

};