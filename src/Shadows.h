#pragma once
#include "GLSL.h"
#include "Program.h"
#include "ShaderManager.h"
#include "Camera.h"

class Shadows
{
private:
	GLuint depthMapFBO;
	const GLuint S_WIDTH = 1024, S_HEIGHT = 1024;
	GLuint depthMap;
	shared_ptr<Program> prog = NULL;

public:
	Shadows() 
	{
		ShaderManager shaderMan = ShaderManager::GetInstance();
		prog = shaderMan.GetShader("ShadowDepth");
	}
	~Shadows();
	void Shadows::RenderShadowPass();
	void Shadows::InitializeShadowFBO();
};