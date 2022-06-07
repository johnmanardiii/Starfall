#include "Renderer.h"
#include "ComponentManager.h"
void Renderer::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetGameObject(Name);
	//base class assumes there is only one transform component. Pretty safe assumption,
	//define your own init if this is different.
	size_t index = obj.GetComponentLocation("Transform");
	trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));
	cm = compMan;
}

bool Renderer::IsInViewFrustum(const GameState& state, ComponentManager* compMan, const Camera& camera)
{
	const vec3& center = trans->GetPos();
	float dist;
	auto& planes = camera.getVFCPlanes();
	if (isCullable) {
		
		for (int i = 0; i < 6; i++) {
			dist = distToPlane(planes[i].x, planes[i].y, planes[i].z, planes[i].w, center);
			if (dist < 0 && length(dist) > cullingRadius) {
				return false;
			}
		}
		return true;
	}
	else {
		return true; //always considered to be in view frustum if the object is designated as not cullable.
	}
}

float Renderer::distToPlane(float A, float B, float C, float D, const vec3& point)
{
	return A * point.x + B * point.y + C * point.z + D;
}
