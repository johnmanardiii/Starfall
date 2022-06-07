#pragma once
class ComponentManager;
#include "Renderer.h"
#include "Shape.h"
#include "ShaderManager.h"
#include "GameState.h"
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
	void Update(float frameTime, GameState* gameState);
	void Init(GameState* gameState);
	void InitQuad();
	void RenderHUD(int width, int height, GameState* gameState);
	void SetUpFrameBuffers(int width, int height);
	void DrawSprite(GLuint tex, vec2 position, vec2 size, float rotate);
private:
	GLuint starFragment_tex, moonBar_tex, moonIcon_tex, slash_tex;
	shared_ptr<Program> prog;
	GLuint quadVAO;
	mat4 projection;
	shared_ptr<Shape> quad;
	float moonIconPosition = 50;
	float moonIconStartingPos = 70, moonIconEndingPos = 650;
	GLuint starsCollected_onesPlace_tex, starsCollected_tenthPlace_tex;
	GLuint starsRequired_onesPlace_tex, starsRequired_tenthPlace_tex;
	GLuint win_tex, lose_tex, x_tex;
};