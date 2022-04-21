#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> //for value_ptr

#include "Component.h"
#include "Renderer.h"
#include "TextureRenderer.h"
#include "GameObject.h"
#include <vector>
#include <memory>
#include <string>

using glm::vec3;
using glm::mat4;
using glm::normalize;


class Player
{
private:
	Player(vec3 pos);
	vec3 pos = vec3(0);
	float movementSensitivity = 0.05f;
	shared_ptr<Transform> trans = NULL;
public:
	const std::string pName = "player";
	void Update(float frameTime, ComponentManager* compMan);
	bool IsWASDPressed[4] = { false, false, false, false };
	void ProcessWASDInput();
	void Init(ComponentManager* compMan);
	static Player& GetInstance(vec3 pos) {
		static Player instance(pos);
		return instance;
	}
};