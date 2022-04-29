#pragma once
#include "Program.h"
#include <memory>

class Bloom
{
private:
	const static int num_downsamples = 8;
	GLuint bloomTex;
	GLuint downsampleFBOs[num_downsamples];
	GLuint downsampledTex[num_downsamples];
	GLuint upsampledFBOs[num_downsamples];
	GLuint upsampledTex[num_downsamples];
	GLuint bloomFBO;	// if I remember correctly, you can't write to the FBO you are reading from, and multiple passes.
	std::shared_ptr<Program> bloomThresholdProg;
	std::shared_ptr<Program> bloomDownsample;
	std::shared_ptr<Program> bloomUpsample;
	void InitializeShaders();
	void InitializeFramebuffers(int width, int height);
	void DownSample(GLuint quad_vao, int width, int height);
	void Upsample(GLuint quad_vao, int width, int height);
public:
	GLuint GetBloomTex() const { return upsampledTex[0]; }
	Bloom(int width, int height);
	~Bloom();
	void RenderBloom(GLuint quad_vao, GLuint screenTexture, int width, int height);
};