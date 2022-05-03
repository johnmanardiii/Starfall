#include "Renderer.h"
#include "ComponentManager.h"
void Renderer::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetGameObject(Name);
	//base class assumes there is only one transform component. Pretty safe assumption,
	//define your own init if this is different.
	size_t index = obj.GetComponentLocation("Transform").at(0);
	trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));
}