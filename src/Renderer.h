#pragma once
#include "Program.h"
#include "Component.h"
#include "ComponentManager.h"
#include <memory>


using namespace std;
class Renderer : Component
{
public:
	virtual void Draw() = 0;
	virtual void Init(ComponentManager&);
	Renderer(string gameObject) : Component(gameObject) {}
protected:
	shared_ptr<Program> prog = NULL;
	shared_ptr<Transform> trans = NULL;

private:
};