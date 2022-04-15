#include "Renderer.h"
#include "ComponentManager.h"
void Renderer::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetObject(Name);
	size_t index = obj.GetComponentLocation("Transform");
	trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));
}