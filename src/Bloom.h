#pragma once
#include "Program.h"
#include "PostProcessing.h"
#include <memory>

class PostProcessing;

// Bloom is the class that uses the screen texture to render Bloom
// Uses a method described by Jorge Jiminez for Call of Duty, and Unity also uses it
// Renders the threshold version of the scene down into 
class Bloom
{
private:
	const static int num_downsamples = 9;
	PostProcessing* postProcessing;
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
	void DownSample();
	void Upsample();
public:
	GLuint GetBloomTex() const { return upsampledTex[0]; }
	Bloom(PostProcessing* _pp);
	~Bloom();
	void RenderBloom();
};