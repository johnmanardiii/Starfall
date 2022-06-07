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
	vec3 shadowColor = vec3(0.33, 0.25, 0.60);
	vec3 terrainColor = vec3(0.76, 0.61, 1.0);
	float sandStrength = 0.08;

	// rim 
	float rimStrength = 0.176;
	float rimPower = 0.96;
	vec3 rimColor = vec3(1, 0.74, 1.0);

	// ocean spec
	float oceanSpecularStrength = 1.0;
	float oceanSpecularPower = 50;
	vec3 oceanSpecularColor = vec3(0.81, 0.37, 1.0);

	// sand ripples
	float steepnessSharpnessPower = 20.0;
	float specularHardness = 2.0;
	float sandRipplesStrength = 1.99f;

	static const float OFFSET_FROM_CENTER;
	static const float TERRAIN_SIZE;
	static const float BASE_HEIGHT;
	void UpdatePosition();
};
