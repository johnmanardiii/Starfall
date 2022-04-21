#include "PlayerTransform.h"


void PlayerTransform::CalcModelMat()
{
	mat4 i = mat4(1.0f);
	mat4 T = glm::translate(i, position),
		R = mat4(rotation),
		S = glm::scale(i, scale);
	parentTransform->CalcModelMat();
	M = parentTransform->GetM() * T * R * S;
}

PlayerTransform::PlayerTransform(std::string name, std::shared_ptr<Transform> parent) : Transform(name)
{
	parentTransform = parent;
}