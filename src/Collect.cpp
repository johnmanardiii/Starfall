#include "Collect.h"
#include "ComponentManager.h"
#include <iostream>
void Collect::Update(float frameTime, ComponentManager* compMan)
{
	// mult by flash speed to be seconds
	if (collision->IsCollected()) {
		BeginFlash();
		//start self-destruct.
		flashAmount += flashSpeed * frameTime;
		renderer->SetFlashAmt(sin(flashAmount));
	}
	
	if (flashAmount > 5 * flashSpeed)
	{
		cout << "Deleting self" << endl;
		compMan->RemoveGameObject(Name);
	}
}

void Collect::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetObject(Name);
	int index = obj.GetComponentLocation("Renderer");
	renderer = static_pointer_cast<TextureRenderer>(compMan->GetComponent("Renderer", index));

	index = obj.GetComponentLocation("Collision");
	collision = static_pointer_cast<Collision>(compMan->GetComponent("Collision", index));
}