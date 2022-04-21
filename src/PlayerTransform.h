#pragma once
#include "Transform.h"
#include <memory>


class PlayerTransform : public Transform
{
public:
	PlayerTransform(std::string name, std::shared_ptr<Transform> parent);
	virtual void CalcModelMat();
private:
	std::shared_ptr<Transform> parentTransform;
};

