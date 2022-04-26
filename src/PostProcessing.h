#pragma once
#include "WindowManager.h"

class PostProcessing
{
private:
	int width, height;
	WindowManager* windowManager;

	GLuint base_fbo;
public:
	const int get_width() const { return width; }	// getters for framebuffer data
	const int get_height() const { return height; }
	PostProcessing(WindowManager* wm);
	~PostProcessing();
	void ClearFramebuffers();
};