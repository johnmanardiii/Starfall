#include "PostProcessing.h"
#include <string>
#include <iostream>

using namespace std;

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

void PostProcessing::InitializeShaders()
{
	std::string resourceDir = "../resources";
	simple_prog = make_shared<Program>();
	simple_prog->setVerbose(true);
	simple_prog->setShaderNames(resourceDir + "/post_vert.glsl", resourceDir + "/post_simple_frag.glsl");
	simple_prog->Init();
	simple_prog->addAttribute("vertPos");
	simple_prog->addAttribute("texCoord");

	GLuint TexLocation = glGetUniformLocation(simple_prog->pid, "screenTexture");
	glUseProgram(simple_prog->pid);
	glUniform1i(TexLocation, 0);
}

PostProcessing::PostProcessing(WindowManager* wm)
{
	windowManager = wm;
	InitializeQuad();
	InitializeShaders();

	// initialize default size of framebuffer textures
	// Get current frame buffer size.
	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, width, height);
	// Clear default framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

	glGenRenderbuffers(1, &base_depth_stencil);
	glBindRenderbuffer(GL_RENDERBUFFER, base_depth_stencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, base_depth_stencil);

	// check if framebuffer is set up properly:
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glViewport(0, 0, width, height);
	// Clear default framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

	// clear the base framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, base_fbo);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// clear the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// set render target to default offscreen framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, base_fbo);
	glEnable(GL_DEPTH_TEST);
}

void PostProcessing::RenderPostProcessing()
{
	glDisable(GL_DEPTH_TEST);
	// bind default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	simple_prog->bind();
	glBindVertexArray(quad_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, base_color);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	simple_prog->unbind();
}