#include "Collect.h"
#include "ComponentManager.h"
#include "ParticleStaticSplash.h"
#include <iostream>
void Collect::Update(float frameTime, ComponentManager* compMan)
{
	// mult by flash speed to be seconds
	if (collision->IsCollected()) {
		//start self-destruct.
		BeginCollect();
		timeElapsed += animSpeed * frameTime;
		//start taking over the other components.
		particle->IsActive = true;
		collision->IsActive = false; // now it doesn't collide with anything.
		transform->ApplyRotation(frameTime * 16, vec3(0, 0, 1));
	}
	
	//delete after 6s
	if (timeElapsed > 6 * animSpeed) {
		compMan->RemoveGameObject(Name);
	}
	//disable particle effects after 3s
	else if (timeElapsed > 3 * animSpeed)
	{   
		particle->IsActive = false;
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

	index = obj.GetComponentLocation("Particle");
	particle = static_pointer_cast<ParticleStaticSplashRenderer>(compMan->GetComponent("Particle", index));
	particle->IsActive = false;
}
