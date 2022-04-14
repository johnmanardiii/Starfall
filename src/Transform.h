#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


class Transform : public Component
{
public:
	Transform(std::string name) : Component(name) {}

	vec3 GetPos();
	quat GetRot();
	vec3 GetScale();

	void SetPos(vec3);
	void SetRot(quat);
	void SetScale(vec3);

	void ApplyTranslation(vec3);
	void ApplyRotation(float, vec3);
	void ApplyScale(vec3);

	void Update();
	void Init(ComponentManager& manager);

private:
	vec3 position;
	quat rotation;
	vec3 scale;
};

