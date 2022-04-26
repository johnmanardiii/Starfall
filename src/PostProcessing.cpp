#include "PostProcessing.h"

PostProcessing::PostProcessing(WindowManager* wm)
{
	windowManager = wm;

	// initialize default size of framebuffer textures
	// Get current frame buffer size.
	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
	glViewport(0, 0, width, height);
	// Clear default framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Initialize offscreen render targets
	glGenFramebuffers(1, &base_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, base_fbo);
}

PostProcessing::~PostProcessing()
{
	glDeleteFramebuffers(1, &base_fbo);
}

void PostProcessing::ClearFramebuffers()
{
	//TODO: check for width/height change and adjust the textures.
	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set render target to default offscreen framebuffer
}