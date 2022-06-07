#pragma once
#include "Renderer.h"
#include "Shape.h"
#include "ShaderManager.h"
#include "ComponentManager.h"
#include "DroneManager.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //for lookat
#include <glm/gtc/type_ptr.hpp> //for value_ptr

class DroneRenderer : public Renderer
{
public:
	DroneRenderer(string mod, string baseTex, string emissionTex,
		string normalTex, string gameObject) : Renderer(gameObject)
	{
		ShaderManager shaderMan = ShaderManager::GetInstance();
		prog = shaderMan.GetShader("drone");
		base_tex = shaderMan.GetTexture(baseTex);
		emissive_tex = shaderMan.GetTexture(emissionTex);
		normal_tex = shaderMan.GetTexture(normalTex);
		model = shaderMan.GetModel(mod);
	}
	void Update(float frameTime, ComponentManager& compMan) { Draw(frameTime); }
	void Init(ComponentManager* compMan);

	void Draw(float frameTime);

private:
	shared_ptr<Shape> model;
	shared_ptr<DroneManager> drone_manager;
	GLuint base_tex, emissive_tex, normal_tex;
};