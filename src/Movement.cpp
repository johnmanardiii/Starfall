#include "Movement.h"
#include "ComponentManager.h"
vec3 Movement::GetVel()
{
	return velocity;
}

void Movement::SetVel(vec3 vel)
{
	velocity = vel;
}

void Movement::AddVel(vec3 vel)
{
	velocity += vel;
}

vec3 Movement::GetAccel()
{
	return accel;
}

void Movement::SetAccel(vec3 newAccel)
{
	accel = newAccel;
}

void Movement::AddAccel(vec3 accelToAdd)
{
	accel += accelToAdd;
}

void Movement::Move(float frameTime)
{
	trans->ApplyTranslation(velocity * frameTime);
}

void Movement::Accelerate(float frameTime)
{
	velocity += accel * frameTime;
}

void Movement::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetGameObject(Name);
	int index = obj.GetComponentLocation("Transform");
	trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));
}

void Movement::Update(float frameTime, ComponentManager* compMan)
{
	velocity += accel * frameTime;
	Move(frameTime);
}