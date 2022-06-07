#pragma once
#include "Transform.h"
#include <memory>


class EulerTransform : public Transform
{
public:
	EulerTransform(std::string name);
	virtual void CalcModelMat();
	void SetRoll(float roll) { rollAmount = roll; }
	void SetLean(float lean) { leanAmount = lean; }
	void SetVisualOffset(float offset) { visualHeightOffset = offset; }
private:
	float rollAmount = 0.0f;	// rotation around the z axis (degrees)
	float leanAmount = 0.0f;	// rotation around the x axis
	float visualHeightOffset = 0.0f;
};

