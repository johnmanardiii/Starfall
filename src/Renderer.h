#pragma once
class ComponentManager;
#include "Program.h"
#include "GameState.h"
#include "Camera.h"
#include "Transform.h"

#include "Component.h"
#include <memory>


using namespace std;
class Renderer : public Component
{
public:
	virtual void Draw(float frameTime) = 0;
	virtual void Init(ComponentManager*);
	virtual void Update(float frameTime, ComponentManager* compMan) { Draw(frameTime); }
	virtual bool IsInViewFrustum(const GameState& state, ComponentManager* compMan, const Camera& camera);
	bool isCullable = false; //objects will, by default, not be culled. Set this in a derived Init or Update.
	
	Renderer(string gameObject) : Component(gameObject) {}
protected:
	shared_ptr<Program> prog = NULL;
	shared_ptr<Transform> trans = NULL;
	float cullingRadius;
	float distToPlane(float A, float B, float C, float D, const vec3& point);
private:
};