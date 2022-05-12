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
		shallowTexture = shaderMan.GetTexture("sandShallow");
		steepTexture = shaderMan.GetTexture("sandSteep");

		terrain = shaderMan.Terrain;
	}
	void Update(float frameTime, ComponentManager& compMan) { }//Draw(frameTime); } I commented this out because its a renderer component.
	void Init(ComponentManager* compMan);
	void UpdateUniforms();
	void Draw(float frameTime);
private:
	TerrainS terrain;
	GLuint texture;
	GLuint heightTexture;
	GLuint noiseTexture;
	GLuint shallowTexture;
	GLuint steepTexture;
};
