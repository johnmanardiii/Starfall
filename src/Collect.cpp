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
		rendererExplosion->IsActive = false;
		particle->IsActive = false;
	} 

}

void Collect::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetGameObject(Name);
	vector<size_t> renderIndices = obj.GetComponentLocation("Renderer");
	rendererObject = static_pointer_cast<StarRenderer>(compMan->GetComponent("Renderer", renderIndices.at(0)));
	rendererExplosion = static_pointer_cast<StarRenderer>(compMan->GetComponent("Renderer", renderIndices.at(1)));


	int index = obj.GetComponentLocation("Collision").at(0);
	collision = static_pointer_cast<Collision>(compMan->GetComponent("Collision", index));

	index = obj.GetComponentLocation("Transform").at(0);
	transform = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));

	index = obj.GetComponentLocation("Particle").at(0);
	particle = static_pointer_cast<ParticleStaticSplashRenderer>(compMan->GetComponent("Particle", index));
	particle->IsActive = false;
}
