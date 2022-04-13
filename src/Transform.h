#pragma once
#include "Component.h"
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


class Transform : public Component
{
public:
	vec3 GetPos();
	quat GetRot();
	vec3 GetScale();

	void SetPos(vec3);
	void SetRot(quat);
	void SetScale(vec3);

	void ApplyTranslation(vec3);
	void ApplyRotation(float, vec3);
	void ApplyScale(vec3);

private:
	vec3 _position;
	quat _rotation;
	vec3 _scale;
};

