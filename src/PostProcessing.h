#pragma once
#include "WindowManager.h"
#include "Program.h"
#include "Bloom.h"
#include "MotionBlur.h"
#include "Camera.h"
#include <memory>

class Bloom;
class MotionBlur;

class PostProcessing
{
private:
	int width, height;
	WindowManager* windowManager;
	Camera* camera;
	void InitializeQuad();
	void InitializeShaders();
	GLuint base_fbo;
	GLuint quad_vao, quad_vbo;
	GLuint base_color, base_depth;
	std::shared_ptr<Program> simple_prog;
	std::shared_ptr<Bloom> bloom;
	std::shared_ptr<MotionBlur> mb;
public:
	const int get_width() const { return width; }	// getters for framebuffer data
	const int get_height() const { return height; }
	const GLuint get_quad_vao() const { return quad_vao; }
	const GLuint get_base_texture() const { return base_color; }
	const GLuint get_motion_blur_texture() const { return mb->GetMotionBlurTex(); }
	const GLuint GetDepthTexture() const { return base_depth; }
	PostProcessing(WindowManager* wm, Camera* cam);
	~PostProcessing();
	void ClearFramebuffers();
	void RenderPostProcessing();
};