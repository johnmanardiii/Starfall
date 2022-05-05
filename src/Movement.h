#pragma once
#include "GameObject.h"
#include <glm/gtc/type_ptr.hpp>
#include "Transform.h"
class ComponentManager;

#include <memory>
using namespace glm;
class Movement : public Component
{
public:
	Movement(std::string name) : Component(name), velocity(vec3(0)), accel(vec3(0)) {}
	Movement(std::string name, vec3 vel) : Component(name), velocity(vel), accel(vec3(0)) {}
	vec3 GetVel();
	vec3 GetAccel();
	
	void SetVel(vec3);
	void SetAccel(vec3);

	void AddVel(vec3);
	void AddAccel(vec3);

	void Update(float frameTime, ComponentManager* compMan);
	void Init(ComponentManager*);
	void virtual Move(float frameTime);
	void virtual Accelerate(float frameTime);
protected:
	std::shared_ptr<Transform> trans;
	vec3 velocity;
	vec3 accel = vec3(0);

private:
	
};