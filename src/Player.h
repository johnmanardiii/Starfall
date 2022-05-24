#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> //for value_ptr

#include "Component.h"
#include "Renderer.h"
#include "TextureRenderer.h"
#include "GameObject.h"
#include "EulerTransform.h"
#include "PlayerMovement.h"
#include "PlayerTransform.h"
#include "imgui/imgui.h"

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
	float playerYOffset = -1.5f;	// target move pos
	float currentFloatOffset = 0;
	float idleTime = 0.0f;
	const float idleHeight = .4f;
	float currentRotationChange = 0.0f;
	float currentZRotation = 0.0f;
	float currentXRotation = 0.0f;
	float radius = 1.0f;
	void AnimatePlayerModel(float frameTime);
	void AddIdleOffset(float frameTime);
	void SetManualRotations();
	void SetAutomaticRotations(float frameTime);
	shared_ptr<Transform> trans = NULL;
	shared_ptr<EulerTransform> pTransform;
	shared_ptr<Transform> headTrans;
	shared_ptr<PlayerTransform> arm1Trans;
	shared_ptr<PlayerTransform> arm2Trans;
	shared_ptr<PlayerMovement> movement = NULL;
	void UpdatePlayerAnimations(float frameTime);
	vec3 rightArmEulerOffset = vec3(0, 0, 0);
	vec3 leftArmEulerOffset = vec3(0, 0, 0);


	// animation constants (keyframes)
	vec3 rightArmBoostForward = vec3(-39, 0, -11);
	vec3 leftArmBoostForward = vec3(-39, 0, 11);
	vec3 rightArmTurnOut = vec3(0, 0, 20);
	vec3 leftArmTurnOut = vec3(0, 0, -20);
	vec3 rightArmBoostBackward = vec3(39, 0, -11);
	vec3 leftArmBoostBackward = vec3(39, 0, 11);
	vec3 rightArmIdle = vec3(0, 33, -9);
	vec3 leftArmIdle = vec3(0, -33, 9);

	quat rBoostForwards, lBoostForwards,
		rArmTurnOut, lArmTurnOut,
		rBoostBackwards, lBoostBackwards,
		rArmIdle, lArmIdle;
	quat rRot = glm::identity<quat>();
	quat lRot = glm::identity<quat>();
public:
	const std::string pName = "player";
	const std::string pArm1Name = "arm1";
	const std::string pArm2Name = "arm2";
	const std::string pHeadName = "head";
	void Update(float frameTime, ComponentManager* compMan);
	void SetInput(int index, bool val);
	vec3 GetForward();
	float GetCurrentSpeed() { return movement->GetSpeed(); }
	float GetMaxSpeed() { return movement->GetMaxSpeed(); }
	void Init(ComponentManager* compMan, shared_ptr<EulerTransform> pTrans,
		shared_ptr<PlayerTransform> head, shared_ptr<PlayerTransform> arm1,
		shared_ptr<PlayerTransform> arm2);
	const vec3 GetPosition() { return pTransform->GetPos(); }
  // Is GetRadius Needed?
	float GetRadius() { return radius; }
	static Player& GetInstance(vec3 pos) {
		static Player instance(pos);
		return instance;
	}
};