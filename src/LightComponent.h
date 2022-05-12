#pragma once
#include "Component.h"
#include "Transform.h"

class LightComponent : Component
{
private:
	LightComponent(vec3 sunDir) : Component("LightComponent"), sunDir(sunDir) {}
	void Update(float frameTime, ComponentManager* compMan);
	void Init(ComponentManager* compMan);
public:
	vec3 sunDir = vec3(0, 0, 1);
	float sunRotation = 0; // Sun rotation around x axis
	static LightComponent& GetInstance(vec3 sunDir) {
		static LightComponent instance(sunDir);
		return instance;
	}
};

