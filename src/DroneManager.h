#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include "Player.h"
class DroneManager : public Component
{
public:
	DroneManager(std::string name) : Component(name) {}
	void Update(float frameTime, ComponentManager* compMan);
	void Init(ComponentManager* compMan);
	void AddDrone();
	vector<vec3> GetPositions();
	vector<mat4> GetModelMatrices();
	void OnCollect(int count);

private:
	std::shared_ptr<Transform> trans;
	std::shared_ptr<Player> player;
	vector<vec3> positions;
	vector<mat4> model_matrices;
	vector<vec3> offsets;
	static const float DRONE_SPEED;
	// Maximum drone offset from target
	static const float MAX_DRONE_OFFSET;
	static const float MIN_DRONE_OFFSET;
	static const int STARS_NEEDED_TO_SPAWN_DRONE;

	vec3 CalcNewPos(vec3 current, vec3 target, float frameTime);
};