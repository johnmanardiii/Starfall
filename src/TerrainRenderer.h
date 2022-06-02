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

	vec3 sunDir = vec3(0.6, 0.01, -0.806);
	float sunRotation = 0;

	// diffuse
	float diffuseContrast = 2;
	vec3 shadowColor = vec3(0.611, 0.44, 0.32);
	vec3 terrainColor = vec3(1, 0.79, 0.65);
	float sandStrength = 0.1;

	// rim 
	float rimStrength = 0.3;
	float rimPower = 20;
	vec3 rimColor = vec3(1, 0.9, 0.94);

	// ocean spec
	float oceanSpecularStrength = 0.3;
	float oceanSpecularPower = 100;
	vec3 oceanSpecularColor = vec3(1, 0.9, 0.94);

	// sand ripples
	float steepnessSharpnessPower = 20.0;
	float specularHardness = 2.0;

	static const float OFFSET_FROM_CENTER;
	static const float TERRAIN_SIZE;
	static const float BASE_HEIGHT;
	void UpdatePosition();
};
