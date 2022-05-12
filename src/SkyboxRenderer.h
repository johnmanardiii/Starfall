#pragma once
class ComponentManager;
#include "Renderer.h"
#include "Shape.h"
#include "ShaderManager.h"
#include "imgui/imgui.h"
#include <memory>
#include <glm/glm.hpp>

class SkyboxRenderer : public Renderer
{
public:
	SkyboxRenderer(string gameObject, string mod) : Renderer(gameObject)
	{
		ShaderManager shaderMan = ShaderManager::GetInstance();
		prog = shaderMan.GetShader("Skybox");
		model = shaderMan.GetModel(mod);

		cloudBaseNoise = shaderMan.GetTexture("cloudBaseNoise");
		cloudNoise = shaderMan.GetTexture("cloudNoise");
		cloudDistort = shaderMan.GetTexture("cloudDistort");
	}
	void Update(float frameTime, ComponentManager& compMan) { Draw(frameTime); }
	void Init(ComponentManager* compMan);
	void Draw(float frameTime);
	void UpdateUniforms();
private:
	shared_ptr<Shape> model;
	vec3 dayTopColor = vec3(0.36, 0.4, 1);
	vec3 dayBottomColor = vec3(0.95, 0.25, 0.14);
	vec3 nightTopColor = vec3(0, 0.01, 0.25);
	vec3 nightBottomColor = vec3(0.03, 0.01, 0.23);
	vec3 horizonColor = vec3(1, 0, 0);
	float texCoordAddOffset = 0;
	float texCoordsMultOffset = 0;
	vec3 sunDir = vec3(0.6, 0.01, -0.806);

	float sunRotation = 0;

	float moonOffset = 0.067;

	float cloudFuzziness = 0.2;
	float cloudScale = 0.2;
	float cloudSpeed = 0.2;
	float cloudCutoff = 0.2;

	vec3 cloudColorDayEdge = vec3(0.36, 0.4, 1);
	vec3 cloudColorDayMain = vec3(0.36, 0.4, 1);
	vec3 cloudColorNightEdge = vec3(0.36, 0.4, 1);
	vec3 cloudColorNightMain = vec3(0.36, 0.4, 1);

	// Textures
	GLuint cloudBaseNoise;
	GLuint cloudNoise;
	GLuint cloudDistort;
};