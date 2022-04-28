#pragma once
#include "Program.h"
#include <memory>

class Bloom
{
private:
	GLuint bloomTex, bloomTex2;
	GLuint bloomFBO, bloomFBO2;	// if I remember correctly, you can't write to the FBO you are reading from, and multiple passes.
	std::shared_ptr<Program> bloomThresholdProg;
	std::shared_ptr<Program> bloomDownscaleProg;
	std::shared_ptr<Program> bloomUpscaleProg;
public:
	Bloom();
	~Bloom();
	void RenderBloom(GLuint quad_vao, GLuint screenTexture);
	GLuint GetBloomTexture();
};