#include "DroneManager.h"

const float DroneManager::DRONE_SPEED = 2.5f;
const float DroneManager::DRONE_WOBBLE = 0.20f;
const float DroneManager::DRONE_WOBBLE_SPEED = 1.0f;
const float DroneManager::MAX_DRONE_OFFSET = 5.5f;
const float DroneManager::MIN_DRONE_OFFSET = 4.0f;
const int DroneManager::STARS_NEEDED_TO_SPAWN_DRONE = 10;
void DroneManager::Init(ComponentManager* compMan)
{
	GameObject obj = compMan->GetGameObject(Name);
	int index = obj.GetComponentLocation("Transform");
	trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));
	positions = vector<vec3>();
	model_matrices = vector<mat4>();
	offsets = vector<vec3>();
	t_vals = vector<float>();
}

void DroneManager::Update(float frameTime, ComponentManager* compMan)
{
	float offsetFromPlayer = 2.0f,
		yOff = 3.0f;
	Player player = Player::GetInstance(vec3(0));
	vec3 target = player.GetPosition();
	for (int i = 0; i < positions.size(); i++)
	{
		vec3 oldPos = positions[i];
		vec3 droneTarget = target + player.GetRotation() * offsets[i];
		vec3 newPos = CalcNewPos(oldPos, droneTarget, frameTime, t_vals[i]);
		// Have drones look in direction of player movement
		// mat4 rot = mat4(rotation(vec3(1.0f, 0.0f, 0.0f), player.GetForward()));
		// Have drones look at Luna at all times
		mat4 rot = mat4(rotation(vec3(1.0f, 0.0f, 0.0f), normalize(target - newPos)));
		mat4 T = glm::translate(mat4(1.0f), newPos);
		model_matrices[i] = T * rot;
		positions[i] = newPos;
		t_vals[i] += frameTime * DRONE_WOBBLE_SPEED;
	}
}

vec3 DroneManager::CalcNewPos(vec3 current, vec3 target, float frameTime, float sint)
{
	float t = DRONE_SPEED * frameTime;
	vec3 newPos = current * (1 - t) + target * t;
	newPos.y += abs(sin(sint)) * DRONE_WOBBLE;
	newPos.y = (std::max)(HeightCalc::heightCalc(newPos.x, newPos.z), newPos.y);
	return newPos;
}

void DroneManager::AddDrone()
{
	Camera cam = Camera::GetInstance(vec3(0));
	positions.push_back(cam.GetPos());
	model_matrices.push_back(mat4(1.0f));
	float angle = -(rand() / (float)RAND_MAX) * pi<float>();
	float radius = (rand() / (float)RAND_MAX) * (MAX_DRONE_OFFSET - MIN_DRONE_OFFSET) + MIN_DRONE_OFFSET;
		// Generate number from -1 to 1
	//   not the most uniform distribution, but that isn't necessary
	float x = cos(angle),
		y = sin(angle),
		z = 0.0f;
	offsets.push_back(vec3(x, y, z) * radius);
	t_vals.push_back(0);
}

vector<vec3> DroneManager::GetPositions()
{
	return positions;
}

vector<mat4> DroneManager::GetModelMatrices()
{
	return model_matrices;
}

void DroneManager::OnCollect(int count)
{
	if (count % STARS_NEEDED_TO_SPAWN_DRONE == 0)
	{
		AddDrone();
	}
}