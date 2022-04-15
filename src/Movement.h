#pragma once
#include "Component.h"
#include "GameObject.h"
#include <glm/gtc/type_ptr.hpp>
#include "Transform.h"
#include "ComponentManager.h"

#include <memory>
using namespace glm;

class Movement : public Component
{
public:
	Movement(std::string name) : Component(name), velocity(vec3(0)) {}
	Movement(std::string name, vec3 vel) : Component(name), velocity(vel) {}
	vec3 GetVel();
	
	void SetVel(vec3);

	void ApplyVel(vec3);

	void Update(float frameTime, ComponentManager* compMan);
	void Init(ComponentManager*);
	void Move(float frameTime);

private:
	
	vec3 velocity;
	std::shared_ptr<Transform> trans;
};