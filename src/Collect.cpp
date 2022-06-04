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
        transform->ApplyScale(vec3(0.80f));

		//play sound a single time.
		compMan->Audio()->PlayClocked("collect_majortriad.mp3");
	}
	
	//delete after 6s
	if (timeElapsed > 2.4 * animSpeed) {
		compMan->RemoveGameObject(Name);
	}
	//disable particle effects after 3s
	else if (timeElapsed > 2 * animSpeed)
	{   
        transform->ApplyScale(vec3(1.1f));
		particle->IsActive = false;
	}
	else if (timeElapsed > 0.3 * animSpeed)
	{
		rendererObject->IsActive = false;
	}

}

void Collect::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetGameObject(Name);
	int renderIndex = obj.GetComponentLocation("Renderer");
	rendererObject = static_pointer_cast<StarRenderer>(compMan->GetComponent("Renderer", renderIndex));
	
    int index = obj.GetComponentLocation("Collision");
	collision = static_pointer_cast<Collision>(compMan->GetComponent("Collision", index));

    index = obj.GetComponentLocation("Transform");
	transform = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));

	index = obj.GetComponentLocation("Particle");
	particle = static_pointer_cast<ParticleRenderer>(compMan->GetComponent("Particle", index));
	particle->IsActive = false;
}
