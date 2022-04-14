#include "Transform.h"

vec3 Transform::GetPos() { return position; }
quat Transform::GetRot() { return rotation; }
vec3 Transform::GetScale() { return scale; }

void Transform::SetPos(vec3 newPos) 
{
	position = newPos; 
}

void Transform::SetRot(quat newRot) 
{ 
	rotation = newRot; 
}

void Transform::SetScale(vec3 newScale) 
{ 
	// NOTE: should we add somthing to note when the scale is < 0 in a direction?
	scale = newScale; 
}

void Transform::ApplyTranslation(vec3 offset)
{
	position += offset;
}

void Transform::ApplyRotation(float degrees, vec3 axes)
{
	rotation = glm::rotate(rotation, degrees, axes);
}

void Transform::ApplyScale(vec3 scalars) 
{
	// NOTE: Should we add some notification that the scale would be <= 0 after applying scalars?
	scale = scale * scalars;
}

void Transform::Update(float frameTime) {}

void Transform::Init(ComponentManager& compMan) {}