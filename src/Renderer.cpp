#include "Renderer.h"
#include "ComponentManager.h"
void Renderer::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetGameObject(Name);
	size_t index = obj.GetComponentLocation("Transform");
	trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));
}