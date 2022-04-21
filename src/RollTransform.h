#pragma once
#include "Transform.h"
#include <memory>


class RollTransform : public Transform
{
public:
	RollTransform(std::string name);
	virtual void CalcModelMat();
	void SetRoll(float roll) { rollAmount = roll; }
private:
	float rollAmount = 0.0f;
};

