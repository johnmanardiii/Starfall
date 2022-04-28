#pragma once
#include "WindowManager.h"
#include "Program.h"
#include "Bloom.h"
#include <memory>

class PostProcessing
{
private:
	int width, height;
	WindowManager* windowManager;
	void InitializeQuad();
	void InitializeShaders();
	GLuint base_fbo;
	GLuint quad_vao, quad_vbo;
	GLuint base_color, base_depth_stencil;
	std::shared_ptr<Program> simple_prog;
	std::shared_ptr<Bloom> bloom;
public:
	const int get_width() const { return width; }	// getters for framebuffer data
	const int get_height() const { return height; }
	PostProcessing(WindowManager* wm);
	~PostProcessing();
	void ClearFramebuffers();
	void RenderPostProcessing();
};