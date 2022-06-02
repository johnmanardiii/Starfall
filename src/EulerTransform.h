#pragma once
#include "Transform.h"
#include <memory>


class EulerTransform : public Transform
{
public:
	EulerTransform(std::string name);
	virtual void CalcModelMat();
	void SetRoll(float roll) { rollAmount = roll; }
	void SetPitch(float lean) { leanAmount = lean; }
private:
	float rollAmount = 0.0f;	// rotation around the z axis (degrees)
	float leanAmount = 0.0f;	// rotation around the x axis
};

