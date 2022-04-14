#pragma once
#include "Component.h"
#include "TextureRenderer.h"
#include <memory>
#include <string>

class Collect : public Component
{
public:
	Collect(string name) : Component(name) {}
	void Update(float);
	void Init(ComponentManager&);

	void BeginFlash() { flashSpeed = 10; }
private:
	float flashAmount = 0;
	float flashSpeed = 0;
	shared_ptr<TextureRenderer> renderer;
};