#pragma once
#include "Program.h"
#include "PostProcessing.h"
#include <memory>

class MotionBlur
{
private:
	const static int num_downsamples = 9;
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
public:
	GLuint GetBloomTex() const { return upsampledTex[0]; }
	MotionBlur(PostProcessing* postprocessing);
	~MotionBlur();
	void RenderMotionBlur(GLuint quad_vao, GLuint screenTexture, int width, int height);
};