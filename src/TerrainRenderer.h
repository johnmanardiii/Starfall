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

class TerrainRenderer : public Renderer
{
public:
	TerrainRenderer(string tex, string heightTex, string gameObject) : Renderer(gameObject)
	{
		ShaderManager shaderMan = ShaderManager::GetInstance();
		prog = shaderMan.GetShader("Height");
		texture = shaderMan.GetTexture(tex);
		heightTexture = shaderMan.GetTexture(heightTex);
		noiseTexture = shaderMan.GetTexture("noiseTex");
		terrain = shaderMan.Terrain;
	}
	void Update(float frameTime, ComponentManager& compMan) { Draw(frameTime); }
	void Init(ComponentManager* compMan);

	void Draw(float frameTime);
private:
	TerrainS terrain;
	GLuint texture;
	GLuint heightTexture;
	GLuint noiseTexture;
};
