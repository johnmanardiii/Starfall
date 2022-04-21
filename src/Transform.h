#pragma once
#include "Component.h"
class ComponentManager;
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;


class Transform : public Component
{
public:
	Transform(std::string name) : Component(name) {}

	vec3 GetPos();
	quat GetRot();
	vec3 GetScale();
	virtual const mat4 GetModelMat() const { return M; };

	void SetPos(vec3);
	void SetRot(quat);
	void SetScale(vec3);

	void ApplyTranslation(vec3);
	void ApplyRotation(float, vec3);
	void ApplyScale(vec3);

	virtual void Update(float frameTime, ComponentManager* compMan);
	void Init(ComponentManager* manager);

	virtual void CalcModelMat();
	vec3 GetForward();
	vec3 GetRight();
	mat4 GetM() { return M; }
protected:
	vec3 position = vec3(0.0f, 0.0f, 0.0f);
	quat rotation = quat(0, 0, 0, 1);
	vec3 scale = vec3(1.0f, 1.0f, 1.0f);
	mat4 M = mat4(1.0f);
};

