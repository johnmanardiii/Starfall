#pragma once
#include "Component.h"
#include "Transform.h"
#include "Shadows.h"

class LightComponent : Component
{
public:
	LightComponent(vec3 sunD) : Component("LightComponent")
	{
		sunDir = sunD;
		ShaderManager shaderMan = ShaderManager::GetInstance();
		depthProg = shaderMan.GetShader("ShadowDepth");
	}
	void Update(float frameTime, ComponentManager* compMan);
	void Init(ComponentManager* compMan);
	static LightComponent& GetInstance(vec3 sunDir) {
		static LightComponent instance(sunDir);
		return instance;
	}
	// Sun rotation around x axis
	float sunRotation = 0;
	vec3 sunDir = vec3(0, 0, 1);
	vec3 moonDir = vec3(0, 0, -1);
	mat4 LightComponent::GetOrthoMatrix();
	mat4 LightComponent::GetLightView();
	shared_ptr<Shadows> shadows = NULL;
	shared_ptr<Program> depthProg = NULL;
private:
	float startSunRotation = 0.65;
	float endSunRotation = -0.4;
	float timeLeft = 60;
	float startTime = 60;
	ComponentManager* componentManager;
};
