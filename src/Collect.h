#pragma once
#include "Component.h"
#include "Collision.h"
#include "StarRenderer.h"
#include <memory>
#include <string>

class ParticleRenderer;

class Collect : public Component
{
public:
	Collect(string name) : Component(name) {}
	void Update(float, ComponentManager*);
	void Init(ComponentManager*);

	void BeginCollect() { animSpeed = 10; }
private:
	float timeElapsed = 0;
	float animSpeed = 0;
	shared_ptr<StarRenderer> rendererObject;
    shared_ptr<Collision> collision;
	shared_ptr<Transform> transform;
	shared_ptr<ParticleRenderer> particle;
};
