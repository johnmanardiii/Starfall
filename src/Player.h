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
	float speed = 9.0f;	// speed in units/second
	float playerYOffset = 2.2f;
	float rotationSpeed = 90.0f;	// degrees per second
	shared_ptr<Transform> trans = NULL;
	void SetPosToGround();

	shared_ptr<Transform> headTrans;
	shared_ptr<Transform> arm1Trans;
	shared_ptr<Transform> arm2Trans;
public:
	const std::string pName = "player";
	const std::string pArm1Name = "arm1";
	const std::string pArm2Name = "arm2";
	const std::string pHeadName = "head";
	void Update(float frameTime, ComponentManager* compMan);
	bool inputBuffer[4] = { false, false, false, false };
	void ProcessWASDInput();
	vec3 GetForward();
	void Init(ComponentManager* compMan, shared_ptr<Transform> head, 
		shared_ptr<Transform> arm1, shared_ptr<Transform> arm2);
	const vec3 GetPosition() { return pos; }
	static Player& GetInstance(vec3 pos) {
		static Player instance(pos);
		return instance;
	}
};