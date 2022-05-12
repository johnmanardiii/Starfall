#include "LightComponent.h"
#include "ComponentManager.h"

void LightComponent::Update(float frameTime, ComponentManager* compMan)
{
	if (compMan->GetGameState()->IsGameEnded()) return;
	if (timeLeft > 0)
	{ 
		timeLeft -= frameTime;
		sunRotation = glm::mix(endSunRotation, startSunRotation, timeLeft / startTime);
		cout << "Time Left Until Moon Sets: " << timeLeft << '\r' << std::flush;
	}
	else {
		compMan->GetGameState()->EndGame();
	}

	// Sun direction
	mat4 rotY = glm::rotate(mat4(1.0f), sunRotation, glm::vec3(1.0f, 0.0f, 0.0f));
	mat4 inverted = glm::inverse(rotY);
	sunDir = normalize(glm::vec3(inverted[2]));
}

void LightComponent::Init(ComponentManager* compMan)
{
}