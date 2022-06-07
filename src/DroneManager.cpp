#include "DroneManager.h"

const float DroneManager::DRONE_SPEED = 3.0f;

void DroneManager::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetGameObject(Name);
	int index = obj.GetComponentLocation("Transform");
	trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));
	positions = vector<vec3>();
	model_matrices = vector<mat4>();
	AddDrone();
}

void DroneManager::Update(float frameTime, ComponentManager* compMan)
{
	float offsetFromPlayer = 2.0f,
		yOff = 3.0f;
	Player player = Player::GetInstance(vec3(0));
	vec3 target = player.GetPosition() + offsetFromPlayer * player.GetForward();
	target.y = HeightCalc::heightCalc(target.x, target.z) + yOff;
	for (int i = 0; i < positions.size(); i++)
	{
		vec3 oldPos = positions[i];
		vec3 newPos = CalcNewPos(oldPos, target, frameTime);
		mat4 rot = mat4(rotation(vec3(1.0f, 0.0f, 0.0f), normalize(newPos-oldPos)));
		mat4 T = glm::translate(mat4(1.0f), newPos);
		model_matrices[i] = T * rot;
		positions[i] = newPos;
	}
}

vec3 DroneManager::CalcNewPos(vec3 current, vec3 target, float frameTime)
{
	float t = DRONE_SPEED * frameTime;
	vec3 newPos = current * (1 - t) + target * t;
	newPos.y = (std::max)(HeightCalc::heightCalc(newPos.x, newPos.z), newPos.y);
	return newPos;
}

void DroneManager::AddDrone()
{
	Player player = Player::GetInstance(vec3(0));
	positions.push_back(player.GetPosition());
	model_matrices.push_back(mat4(1.0f));
}

vector<vec3> DroneManager::GetPositions()
{
	return positions;
}

vector<mat4> DroneManager::GetModelMatrices()
{
	return model_matrices;
}