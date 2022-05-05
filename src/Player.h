#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> //for value_ptr

#include "Component.h"
#include "Renderer.h"
#include "TextureRenderer.h"
#include "GameObject.h"
#include "EulerTransform.h"
#include "PlayerMovement.h"

#include <vector>
#include <memory>
#include <string>

using glm::vec3;
using glm::mat4;
using glm::normalize;

#define MAX_SLOPE_MULT 20.0f
#define GRAVITY_MULT 10.0f
#define STRONG_GRAVITY_MULT 20.0f

class Player
{
private:
	Player(vec3 pos);
	vec3 pos = vec3(0);
	float playerYOffset = -1.5f;	// target move pos
	float currentFloatOffset = 0;
	float idleTime = 0.0f;
	const float idleHeight = .4f;
	float currentRotationChange = 0.0f;
	float currentZRotation = 0.0f;
	float currentXRotation = 0.0f;
	void AnimatePlayerModel(float frameTime);
	void AddIdleOffset(float frameTime);
	shared_ptr<Transform> trans = NULL;
	shared_ptr<EulerTransform> pTransform;
	shared_ptr<Transform> headTrans;
	shared_ptr<Transform> arm1Trans;
	shared_ptr<Transform> arm2Trans;
	shared_ptr<PlayerMovement> movement = NULL;
public:
	const std::string pName = "player";
	const std::string pArm1Name = "arm1";
	const std::string pArm2Name = "arm2";
	const std::string pHeadName = "head";
	void Update(float frameTime, ComponentManager* compMan);
	void ProcessWASDInput();
	void SetInput(int index, bool val);
	vec3 GetForward();
	void Init(ComponentManager* compMan, shared_ptr<EulerTransform> pTrans,
		shared_ptr<Transform> head, shared_ptr<Transform> arm1,
		shared_ptr<Transform> arm2);
	const vec3 GetPosition() { return pos; }
	static Player& GetInstance(vec3 pos) {
		static Player instance(pos);
		return instance;
	}
};