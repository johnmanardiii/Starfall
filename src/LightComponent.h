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
	vec3 sunDir;
	static LightComponent& GetInstance(vec3 sunDir) {
		static LightComponent instance(sunDir);
		return instance;
	}
};

