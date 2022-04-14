#include "Renderer.h"

void Renderer::Init(ComponentManager& compMan)
{
	GameObject obj = compMan.GetObject(Name);
	size_t index = obj.GetComponentLocation("Transform");
	trans = compMan.GetComponent("Transform", index);
}