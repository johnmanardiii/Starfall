#include "Bloom.h"
#include <iostream>
using namespace std; 

void Bloom::InitializeShaders()
{
	std::string resourceDir = "../resources";
	bloomThresholdProg = make_shared<Program>();
	bloomThresholdProg->setVerbose(true);
	bloomThresholdProg->setShaderNames(resourceDir + "/post_vert.glsl", resourceDir + "/thresh_frag.glsl");
	bloomThresholdProg->Init();
	bloomThresholdProg->addAttribute("vertPos");
	bloomThresholdProg->addAttribute("texCoord");

	GLuint TexLocation = glGetUniformLocation(bloomThresholdProg->pid, "screenTexture");
	glUseProgram(bloomThresholdProg->pid);
	glUniform1i(TexLocation, 0);
}

void Bloom::InitializeFramebuffers(int width, int height)
{
	glGenFramebuffers(1, &bloomFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
	// Create offscreen color texture and bind it to framebuffer
	glGenTextures(1, &bloomTex);
	glBindTexture(GL_TEXTURE_2D, bloomTex);
	// make the texture a floating point texture for HDR
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	// NOTE THAT TOOK ME WAY TO LONG TO FIX, DON'T USE MIPMAP FILTERS IF NO MIPMAP DUHHHHH
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomTex, 0);
	// check if framebuffer is set up properly:
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Bloom::Bloom(int width, int height)
{
	// initialize framebuffers + textures
	InitializeFramebuffers(width, height);
	// intialize shaders
	InitializeShaders();
}

Bloom::~Bloom()
{
	// delete framebuffers + textures
	glDeleteFramebuffers(1, &bloomFBO);
	// TODO: delete used textures.
}

void Bloom::RenderBloom(GLuint quad_vao, GLuint screenTexture)
{
	// clear all framebuffers of color data
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);

	// glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// glClear(GL_DEPTH_BUFFER_BIT);

	glClear(GL_COLOR_BUFFER_BIT);

	// extract out threshold values into bloomTex (bloomFBO)
	bloomThresholdProg->bind();
	glBindVertexArray(quad_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	bloomThresholdProg->unbind();
	// downscale image into bloomTex2 (bloomFBO2)

	// upscale image into bloomTex (bloomFBO)
	
}