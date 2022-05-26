#include "EulerTransform.h"


void EulerTransform::CalcModelMat()
{
	mat4 i = mat4(1.0f);
	mat4 T = glm::translate(i, position),
		R = mat4(rotation),
		S = glm::scale(i, scale);
	mat4 rollMatrix = rotate(mat4(1), glm::radians(rollAmount), vec3(0, 0, -1));
	mat4 leanMatrix = rotate(mat4(1), glm::radians(leanAmount), vec3(1, 0, 0));
	// WARNING, MIGHT BE (0, 0, 1) DEPENDING ON MODEL
	M = T * R * rollMatrix * leanMatrix* S;
}

EulerTransform::EulerTransform(std::string name) : Transform(name){}