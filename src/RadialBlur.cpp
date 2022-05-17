#include "RadialBlur.h"
#include "PostProcessing.h"

#include <cmath>
#include <iostream>
using namespace std;

// This only needs to be called once on initialize and is a helper function for the constructor for
// setting up the motion blur shader.
void RadialBlur::InitializeShaders()
{
#ifdef __APPLE__
    std::string resourceDir = "../../resources";
#else
    std::string resourceDir = "../resources";
#endif
	radialBlurProg = make_shared<Program>();
	radialBlurProg->setVerbose(true);
	radialBlurProg->setShaderNames(resourceDir + "/post_vert.glsl", resourceDir + "/radial_blur.glsl");
	radialBlurProg->Init();
	radialBlurProg->addAttribute("vertPos");
	radialBlurProg->addAttribute("texCoord");

	GLuint TexLocation = glGetUniformLocation(radialBlurProg->pid, "screenTexture");
	glUseProgram(radialBlurProg->pid);
	glUniform1i(TexLocation, 0);
	TexLocation = glGetUniformLocation(radialBlurProg->pid, "depthTexture");
	glUniform1i(TexLocation, 1);
}

// This function sets up the framebuffer and texture necessary for running motion blur
// as a single post-processing pass.
void RadialBlur::InitializeFramebuffers(int width, int height)
{
	int scaleDownFactor = 1;
	glGenFramebuffers(1, &blurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
	// Create offscreen color texture and bind it to framebuffer
	glGenTextures(1, &blurTex);
	glBindTexture(GL_TEXTURE_2D, blurTex);
	// make the texture a floating point texture for HDR
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width / scaleDownFactor, height / scaleDownFactor, 0, GL_RGBA, GL_FLOAT, NULL);
	// NOTE THAT TOOK ME WAY TO LONG TO FIX, DON'T USE MIPMAP FILTERS IF NO MIPMAP DUHHHHH
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTex, 0);
	// check if framebuffer is set up properly:
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Constructor initializes framebuffers and shaders.
RadialBlur::RadialBlur(PostProcessing* pp)
{
	// TODO: make sure we aren't dividing by 0 using log2() from cmath
	// num_downsamples = 
	this->postProcessing = pp;

	// initialize framebuffers + textures
	InitializeFramebuffers(postProcessing->GetWidth(), postProcessing->GetHeight());
	// intialize shaders
	InitializeShaders();
}

void RadialBlur::OnResizeWindow()
{
	// delete framebuffers + textures
	glDeleteFramebuffers(1, &blurFBO);
	glDeleteTextures(1, &blurTex);

	InitializeFramebuffers(postProcessing->GetWidth(), postProcessing->GetHeight());
}

RadialBlur::~RadialBlur()
{
	// delete framebuffers + textures
	glDeleteFramebuffers(1, &blurFBO);
	glDeleteTextures(1, &blurTex);
}


// Renders motion blur using the base color and depth texture in PostProcessing into
// blurTex on blurFBO.
void RadialBlur::RenderRadialBlur()
{
	// clear all framebuffers of color data
	glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, postProcessing->GetWidth(), postProcessing->GetHeight());
	// extract out threshold values into bloomTex (bloomFBO)
	radialBlurProg->bind();
	glBindVertexArray(postProcessing->GetQuadVAO());
	// upload inverse of the PV matrix to the program to recreate the world space positions.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, postProcessing->GetLastProcessedScreen());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	radialBlurProg->unbind();
	postProcessing->SetLastProcessedScreen(GetRadialBlurTex());
}
