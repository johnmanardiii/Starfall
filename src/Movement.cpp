#include "Movement.h"

vec3 Movement::GetVel()
{
	return velocity;
}

void Movement::SetVel(vec3 vel)
{
	velocity = vel;
}

void Movement::ApplyVel(vec3 vel)
{
	velocity += vel;
}


void Movement::Move(float frameTime)
{
	trans->ApplyTranslation(velocity * frameTime);
}

void Movement::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetObject(Name);
	int index = obj.GetComponentLocation("Transform");
	trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));
}

void Movement::Update(float frameTime, ComponentManager* compMan)
{
	Move(frameTime);
}