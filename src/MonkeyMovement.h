#pragma once
#include "Movement.h"
class MonkeyMovement : public Movement {
public:
	MonkeyMovement(std::string name, vec3 vel) : Movement(name, vel) {}
	void Move(float frameTime);
};