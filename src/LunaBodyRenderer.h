#pragma once
class ComponentManager;
#include "Renderer.h"
#include "Shape.h"
#include "ShaderManager.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //for lookat
#include <glm/gtc/type_ptr.hpp> //for value_ptr

#include "Camera.h"

class LunaBodyRenderer : public Renderer
{
public:
	LunaBodyRenderer(string mod, string tex, string gameObject) : Renderer(gameObject)
	{
		ShaderManager shaderMan = ShaderManager::GetInstance();
		prog = shaderMan.GetShader("Luna Body");
		texture = shaderMan.GetTexture(tex);
		emissive_tex = shaderMan.GetTexture("Luna Emissive");
		normal_tex = shaderMan.GetTexture("Luna Normal");
		roughness = shaderMan.GetTexture("Luna Roughness");
		metal = shaderMan.GetTexture("Luna Metal");
		model = shaderMan.GetModel(mod);
	}
	void Update(float frameTime, ComponentManager& compMan) { Draw(frameTime); }
	void Init(ComponentManager* compMan);
	void DrawDepth();

	void Draw(float frameTime);
	void SetFlashAmt(float amt) { flashAmt = amt; }
private:
	shared_ptr<Shape> model;
	GLuint texture, emissive_tex, normal_tex, roughness, metal;
	float flashAmt = 0;
	vec3 flashColor = vec3(1, 0, 1);
};