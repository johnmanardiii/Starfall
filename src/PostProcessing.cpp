#include "PostProcessing.h"

/*
Code for initializing a quad from: https://learnopengl.com/Advanced-OpenGL/Framebuffers

Note: makes positions of each quad vertex a vec2
*/
void PostProcessing::InitializeQuad()
{
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &quad_vao);
	glGenBuffers(1, &quad_vbo);
	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

PostProcessing::PostProcessing(WindowManager* wm)
{
	windowManager = wm;

	// initialize default size of framebuffer textures
	// Get current frame buffer size.
	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
	glViewport(0, 0, width, height);
	// Clear default framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	InitializeQuad();

	// Initialize offscreen render targets
	// I referenced this for how to set fbos up: https://learnopengl.com/Advanced-OpenGL/Framebuffers
	glGenFramebuffers(1, &base_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, base_fbo);
	// Create offscreen color texture and bind it to framebuffer
	glGenTextures(1, &base_color);
	glBindTexture(GL_TEXTURE_2D, base_color);
	// TODO: make this larger than 8 bit texture for hdr.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, base_color, 0);
	// check if framebuffer is set up properly:

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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