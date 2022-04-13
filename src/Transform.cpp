#include "Transform.h"

vec3 Transform::GetPos() { return _position; }
quat Transform::GetRot() { return _rotation; }
vec3 Transform::GetScale() { return _scale; }

void Transform::SetPos(vec3 newPos) 
{
	_position = newPos; 
}

void Transform::SetRot(quat newRot) 
{ 
	_rotation = newRot; 
}

void Transform::SetScale(vec3 newScale) 
{ 
	// NOTE: should we add somthing to note when the scale is < 0 in a direction?
	_scale = newScale; 
}

void Transform::ApplyTranslation(vec3 offset)
{
	_position += offset;
}

void Transform::ApplyRotation(float degrees, vec3 axes)
{
	_rotation = glm::rotate(_rotation, degrees, axes);
}

void Transform::ApplyScale(vec3 scalars) 
{
	// NOTE: Should we add some notification that the scale would be <= 0 after applying scalars?
	_scale = _scale * scalars;
}