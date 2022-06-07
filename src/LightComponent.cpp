#include "LightComponent.h"
#include "ComponentManager.h"

void LightComponent::Update(float frameTime, ComponentManager* compMan)
{
	GameState* gameState = compMan->GetGameState();
	if (gameState->IsGameEnded()) return;
	if (gameState->timeLeft > 0)
	{ 
		gameState->timeLeft -= frameTime;
		sunRotation = glm::mix(endSunRotation, startSunRotation, gameState->timeLeft / gameState->startTime);
		cout << "Time Left Until Moon Sets: " << gameState->timeLeft << '\r' << std::flush;
	}
	else {
		compMan->GetGameState()->EndGame();
	}

	// Sun direction
	//ImGui::SliderFloat("SUN ROTATIon", (float*)&sunRotation, -10.0, 10.0f);

	mat4 rotY = glm::rotate(mat4(1.0f), sunRotation, glm::vec3(1.0f, 0.0f, 0.0f));
	mat4 inverted = glm::inverse(rotY);
	sunDir = normalize(glm::vec3(inverted[2]));
	moonDir = vec3(sunDir.x, sunDir.y, sunDir.z * -1.0f);
}

void LightComponent::Init(ComponentManager* compMan)
{
}