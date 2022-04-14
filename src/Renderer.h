#pragma once
class ComponentManager;
#include "Program.h"

#include "Transform.h"

#include "Component.h"
#include <memory>


using namespace std;
class Renderer : public Component
{
public:
	virtual void Draw(float frameTime) = 0;
	virtual void Init(ComponentManager&);
	virtual void Update(float frameTime) { Draw(frameTime); }
	Renderer(string gameObject) : Component(gameObject) {}
protected:
	shared_ptr<Program> prog = NULL;
	shared_ptr<Transform> trans = NULL;

private:
};