#include "Component.h"
#include <glm/gtc/type_ptr.hpp>
#include "Transform.h"

using namespace glm;

class Movement : public Component
{
public:
	vec3 GetVel();
	
	void SetVel(vec3);

	void ApplyVel(vec3);

	virtual void Update();

	void Move();

private:
	vec3 velocity;
	Transform trans;
};