#pragma once
#include "Program.h"
#include "Camera.h"
#include <memory>

class PostProcessing;

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
	const GLuint GetMotionBlurTex() const { return blurTex; }
	MotionBlur(PostProcessing* _pp);
	~MotionBlur();
	void RenderMotionBlur(Camera* cam);
};