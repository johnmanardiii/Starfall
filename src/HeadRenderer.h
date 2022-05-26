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

class HeadRenderer : public Renderer
{
public:
	HeadRenderer(string mod, string tex, string gameObject) : Renderer(gameObject)
	{
		ShaderManager shaderMan = ShaderManager::GetInstance();
		prog = shaderMan.GetShader("HeadShader");
		texture = shaderMan.GetTexture(tex);
		model = shaderMan.GetModel(mod);
	}
	void Update(float frameTime, ComponentManager& compMan) { Draw(frameTime); }
	void Init(ComponentManager* compMan);

	void Draw(float frameTime);
	void SetFlashAmt(float amt) { flashAmt = amt; }
	vec2 eye1Pos = vec2(.1, .5);
	float eye1Radius = .05;
private:
	shared_ptr<Shape> model;
	GLuint texture;
	float flashAmt = 0;
	vec3 flashColor = vec3(1, 0, 1);
};