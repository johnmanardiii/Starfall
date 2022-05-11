#pragma once
#include "Program.h"
#include "Camera.h"
#include <memory>

class PostProcessing;

// Motion Blur renders camera only motion blur to the its texture using the baseTex
// texture found in PostProcessing. I am choosing not to develop this effect any further
// than camera motion blur, as I think other effects would fit better in our game.
class MotionBlur
{
private:
	const static int num_downsamples = 9;
	PostProcessing* postProcessing;
	GLuint blurTex;
	GLuint blurFBO;
	std::shared_ptr<Program> motionBlurProg;
	void InitializeShaders();
	void InitializeFramebuffers(int width, int height);
public:
	MotionBlur(PostProcessing* _pp);
	~MotionBlur();
	const GLuint GetMotionBlurTex() const { return blurTex; }
	void RenderMotionBlur(Camera* cam);
	void OnResizeWindow();
};