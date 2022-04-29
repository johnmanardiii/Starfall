#pragma once
#include "Program.h"
#include <memory>

class Bloom
{
private:
	GLuint bloomTex, bloomDownsample;
	GLuint bloomFBO, bloomFBO2;	// if I remember correctly, you can't write to the FBO you are reading from, and multiple passes.
	std::shared_ptr<Program> bloomThresholdProg;
	std::shared_ptr<Program> bloomDownscaleProg;
	std::shared_ptr<Program> bloomUpscaleProg;
	void InitializeShaders();
	void InitializeFramebuffers(int width, int height);
public:
	GLuint GetBloomTex() const { return bloomTex; }
	Bloom(int width, int height);
	~Bloom();
	void RenderBloom(GLuint quad_vao, GLuint screenTexture);
};