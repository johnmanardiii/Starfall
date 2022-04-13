#include "Movement.h"

void Movement::Update() 
{
	Move();
}

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


void Movement::Move()
{
	trans.ApplyTranslation(velocity);
}