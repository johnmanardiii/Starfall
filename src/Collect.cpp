#include "Collect.h"
#include "ComponentManager.h"
#include <iostream>
void Collect::Update(float frameTime)
{
	flashAmount += flashSpeed * frameTime;
	renderer->SetFlashAmt(sin(flashAmount));
}

void Collect::Init(ComponentManager& compMan)
{
	std::cout << "Init collect" << endl;
	GameObject obj = compMan.GetObject(Name);
	int index = obj.GetComponentLocation("Renderer");
	renderer = static_pointer_cast<TextureRenderer>(compMan.GetComponent("Renderer", index));
}