#pragma once
#include "Program.h"
#include "PostProcessing.h"
#include <memory>

class PostProcessing;

// Bloom is the class that uses the screen texture to render Bloom
// Uses a method described by Jorge Jiminez for Call of Duty, and Unity also uses it
// Renders the threshold version of the scene down into lower and lower
// resolution textures and then combines these textures using an upsampling
// algorithm. A notable change is that each mipmap has exponential falloff.
class Bloom
{
private:
	const static int num_downsamples = 9;	// num_downsamples can increase/decrease w/ resolution
	PostProcessing* postProcessing;
	GLuint bloomFBO;
	GLuint bloomTex;
	GLuint downsampleFBOs[num_downsamples];
	GLuint downsampledTex[num_downsamples];
	GLuint upsampledFBOs[num_downsamples];
	GLuint upsampledTex[num_downsamples];
	std::shared_ptr<Program> bloomThresholdProg;
	std::shared_ptr<Program> bloomDownsample;
	std::shared_ptr<Program> bloomUpsample;
	void InitializeShaders();
	void InitializeFramebuffers(int width, int height);
	void DownSample();
	void Upsample();
	void DeleteFBOTex();
public:
	GLuint GetBloomTex() const { return upsampledTex[1]; }
	Bloom(PostProcessing* _pp);
	~Bloom();
	void RenderBloom();
	void OnResizeWindow();
};