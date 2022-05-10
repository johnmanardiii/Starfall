#pragma once
#include "WindowManager.h"
#include "Program.h"
#include "Bloom.h"
#include "MotionBlur.h"
#include "Camera.h"
#include <memory>

class Bloom;
class MotionBlur;

// PostProcessing is a class that renders all post processing passes of the framebuffer.
// It sets up all framebuffer objects associated with rendering, and sets up the base framebuffer
// used in ComponentManager for rendering of all objects.
// Clear framebuffer needs to be called before any draw calls to the framebuffer because it binds the base framebuffer
// and clears all of the framebuffers before rendering occurs.
// Current Post-Processing Effects: Bloom and Motion Blur. Motion blur is turned down heavily since I don't like the effect.
class PostProcessing
{
private:
	int width, height;
	WindowManager* windowManager;
	Camera* camera;
	GLuint base_fbo;
	GLuint quad_vao, quad_vbo;
	GLuint base_color, base_depth;
	std::shared_ptr<Program> simple_prog;
	std::shared_ptr<Bloom> bloom;
	std::shared_ptr<MotionBlur> mb;

	void InitializeQuad();
	void InitializeFramebuffers();
	void InitializeShaders();
	void DeleteTexturesFramebuffers();
public:
	PostProcessing(WindowManager* wm, Camera* cam);
	~PostProcessing();

	const int GetWidth() const { return width; }	// getters for framebuffer data
	const int GetHeight() const { return height; }
	const GLuint GetQuadVAO() const { return quad_vao; }
	const GLuint GetBaseTex() const { return base_color; }
	const GLuint GetMotionBlurTex() const { return mb->GetMotionBlurTex(); }
	const GLuint GetDepthTexture() const { return base_depth; }
	void SetUpFrameBuffers();
	void RenderPostProcessing();
};