#pragma once
#include "WindowManager.h"

class PostProcessing
{
private:
	int width, height;
	WindowManager* windowManager;
	void InitializeQuad();
	GLuint base_fbo;
	GLuint quad_vao, quad_vbo;
	GLuint base_color, base_depth;
public:
	const int get_width() const { return width; }	// getters for framebuffer data
	const int get_height() const { return height; }
	PostProcessing(WindowManager* wm);
	~PostProcessing();
	void ClearFramebuffers();
};