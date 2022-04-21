#pragma once
#include "Component.h"
#include "Collision.h"
#include "TextureRenderer.h"
#include <memory>
#include <string>

class ParticleStaticSplashRenderer;

class Collect : public Component
{
public:
	Collect(string name) : Component(name) {}
	void Update(float, ComponentManager*);
	void Init(ComponentManager*);

	void BeginFlash() { flashSpeed = 10; }
private:
	float flashAmount = 0;
	float flashSpeed = 0;
	shared_ptr<TextureRenderer> renderer;
	shared_ptr<Collision> collision;
	shared_ptr<Transform> transform;
	shared_ptr<Movement> movement;
	shared_ptr<ParticleStaticSplashRenderer> particle;
};