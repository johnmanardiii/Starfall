#pragma once
class ComponentManager;
#include "Renderer.h"
#include "Shape.h"
#include "ShaderManager.h"
#include <memory>
#include <glm/glm.hpp>

class SkyboxRenderer : public Renderer
{
public:
	SkyboxRenderer(string gameObject, string mod) : Renderer(gameObject)
	{
		ShaderManager shaderMan = ShaderManager::GetInstance();
		prog = shaderMan.GetShader("Texture");
		model = shaderMan.GetModel(mod);
	}
	void Update(float frameTime, ComponentManager& compMan) { Draw(frameTime); }
	void Init(ComponentManager* compMan);
	void Draw(float frameTime);
private:
	shared_ptr<Shape> model;
};