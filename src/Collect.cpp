#include "Collect.h"
#include "ComponentManager.h"
#include "ParticleStaticSplash.h"
#include <iostream>
void Collect::Update(float frameTime, ComponentManager* compMan)
{
	// mult by flash speed to be seconds
	if (collision->IsCollected()) {
		//start self-destruct.
		BeginFlash();
		flashAmount += flashSpeed * frameTime;
		renderer->SetFlashAmt(sin(flashAmount));
		//start taking over the other components.
		particle->IsActive = true;
		auto t = transform->GetPos();
		cout << "collect for: " << Name << " " << t.x << " " << t.y << " " << t.z << endl;
		movement->IsActive = false; // a really easy way to get something to stop moving. Deletion works as normal.
		collision->IsActive = false; // now it doesn't collide with anything.
		transform->ApplyRotation(frameTime * 16, vec3(0, 0, 1));
	}
	
	//delete after 6s
	if (flashAmount > 6 * flashSpeed) {
		cout << endl << "Deleting self" << endl; 
		compMan->RemoveGameObject(Name);
	}
	//shoot off into the sky after 3s
	else if (flashAmount > 3 * flashSpeed)
	{   
		particle->IsActive = false;
		movement->ApplyVel((0.005f * movement->GetVel()) + vec3(0, 0.05, 0));
		movement->IsActive = true;
	} 

}

void Collect::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetGameObject(Name);
	int index = obj.GetComponentLocation("Renderer");
	renderer = static_pointer_cast<TextureRenderer>(compMan->GetComponent("Renderer", index));

	index = obj.GetComponentLocation("Collision");
	collision = static_pointer_cast<Collision>(compMan->GetComponent("Collision", index));

	index = obj.GetComponentLocation("Transform");
	transform = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));

	index = obj.GetComponentLocation("Movement");
	movement = static_pointer_cast<Movement>(compMan->GetComponent("Movement", index));

	index = obj.GetComponentLocation("Particle");
	particle = static_pointer_cast<ParticleStaticSplashRenderer>(compMan->GetComponent("Particle", index));
	particle->IsActive = false;
}
