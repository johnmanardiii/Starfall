#pragma once
#include "Transform.h"
#include <memory>


class PlayerTransform : public Transform
{
public:
	PlayerTransform(std::string name, std::shared_ptr<Transform> parent);
	virtual void CalcModelMat();
	void SetBaseRotation(mat4 rotation) { baseRotation = rotation; }
private:
	std::shared_ptr<Transform> parentTransform;
	glm::mat4 baseRotation = mat4(1);
};

