#pragma once
class ComponentManager;
#include "Renderer.h"
#include "Shape.h"
#include "ShaderManager.h"
#include "imgui/imgui.h"
#include <memory>
#include <glm/glm.hpp>

class HUDRenderer
{
public:
	HUDRenderer()
	{
		ShaderManager shaderMan = ShaderManager::GetInstance();
		prog = shaderMan.GetShader("HUD");

		starFragment_tex = shaderMan.GetTexture("starFragment");
		moonBar_tex = shaderMan.GetTexture("moonBar");
		moonIcon_tex = shaderMan.GetTexture("moonIcon");
		quad = shaderMan.GetModel("quad");
	}
	void Update(float frameTime) {}
	void Init();
	void InitQuad();
	void RenderHUD(int width, int height);
	void SetUpFrameBuffers(int width, int height);
	void DrawSprite(GLuint tex, vec2 position, vec2 size, float rotate);
private:
	GLuint starFragment_tex, moonBar_tex, moonIcon_tex;
	shared_ptr<Program> prog;
	GLuint quadVAO;
	mat4 projection;
	shared_ptr<Shape> quad;
};