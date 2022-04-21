#include "RollTransform.h"


void RollTransform::CalcModelMat()
{
	mat4 i = mat4(1.0f);
	mat4 T = glm::translate(i, position),
		R = mat4(rotation),
		S = glm::scale(i, scale);
	mat4 rollMatrix = rotate(mat4(1), glm::radians(rollAmount), vec3(0, 0, -1));
	// WARNING, MIGHT BE (0, 0, 1) DEPENDING ON MODEL
	M =  T * R * rollMatrix * S;
}

RollTransform::RollTransform(std::string name) : Transform(name){}