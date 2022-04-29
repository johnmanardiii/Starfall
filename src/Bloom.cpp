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

	// set up the downsampling program
	bloomDownsample = make_shared<Program>();
	bloomDownsample->setVerbose(true);
	bloomDownsample->setShaderNames(resourceDir + "/post_vert.glsl", resourceDir + "/downsample_frag.glsl");
	bloomDownsample->Init();
	bloomDownsample->addAttribute("vertPos");
	bloomDownsample->addAttribute("texCoord");
	TexLocation = glGetUniformLocation(bloomDownsample->pid, "threshTex");
	glUseProgram(bloomDownsample->pid);
	glUniform1i(TexLocation, 0);

	// set up the upsampling program
	bloomUpsample = make_shared<Program>();
	bloomUpsample->setVerbose(true);
	bloomUpsample->setShaderNames(resourceDir + "/post_vert.glsl", resourceDir + "/upsample_frag.glsl");
	bloomUpsample->Init();
	bloomUpsample->addAttribute("vertPos");
	bloomUpsample->addAttribute("texCoord");
	TexLocation = glGetUniformLocation(bloomUpsample->pid, "lowRes");
	glUseProgram(bloomUpsample->pid);
	glUniform1i(TexLocation, 0);
	TexLocation = glGetUniformLocation(bloomUpsample->pid, "currentRes");
	glUseProgram(bloomUpsample->pid);
	glUniform1i(TexLocation, 1);
}

void Bloom::InitializeFramebuffers(int width, int height)
{
	int scaleDownFactor = 2;
	glGenFramebuffers(1, &bloomFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
	// Create offscreen color texture and bind it to framebuffer
	glGenTextures(1, &bloomTex);
	glBindTexture(GL_TEXTURE_2D, bloomTex);
	// make the texture a floating point texture for HDR
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width / scaleDownFactor, height / scaleDownFactor, 0, GL_RGBA, GL_FLOAT, NULL);
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

	// set up second bloom frame buffer object
	// generate 5 different FBOS (lolol) rlly? but ok..
	glGenFramebuffers(num_downsamples, downsampleFBOs);
	// Create offscreen color texture and bind it to framebuffer
	glGenTextures(num_downsamples, downsampledTex);
	for (int i = 0; i < num_downsamples; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, downsampleFBOs[i]);	// threshold
		glBindTexture(GL_TEXTURE_2D, downsampledTex[i]);
		// make the texture a floating point texture for HDR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width / ((i + 2) * scaleDownFactor), height / ((i + 2) * scaleDownFactor), 0, GL_RGBA, GL_FLOAT, NULL);
		// NOTE THAT TOOK ME WAY TO LONG TO FIX, DON'T USE MIPMAP FILTERS IF NO MIPMAP DUHHHHH
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downsampledTex[i], 0);
		// check if framebuffer is set up properly:
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
	}

	// generate upsample textures and FBOs
	glGenFramebuffers(num_downsamples, upsampledFBOs);
	// Create offscreen color texture and bind it to framebuffer
	glGenTextures(num_downsamples, upsampledTex);
	for (int i = 0; i < num_downsamples; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, upsampledFBOs[i]);	// threshold
		glBindTexture(GL_TEXTURE_2D, upsampledTex[i]);
		// make the texture a floating point texture for HDR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width / ((i + 2) * scaleDownFactor), height / ((i + 2) * scaleDownFactor), 0, GL_RGBA, GL_FLOAT, NULL);
		// NOTE THAT TOOK ME WAY TO LONG TO FIX, DON'T USE MIPMAP FILTERS IF NO MIPMAP DUHHHHH
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, upsampledTex[i], 0);
		// check if framebuffer is set up properly:
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
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

void Bloom::DownSample(GLuint quad_vao, int width, int height)
{
	bool first = true;
	// start off by rendering a single downsample into FBO2
	for (int i = 0; i < num_downsamples; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, downsampleFBOs[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downsampledTex[i], 0);	// attach tex to framebuffer
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, width / (2 * (i + 2)), height / (2 * (i + 2)));	// set viewport to texture size (hopefully)
		bloomDownsample->bind();
		glBindVertexArray(quad_vao);
		glActiveTexture(GL_TEXTURE0);
		if (first)
		{
			glBindTexture(GL_TEXTURE_2D, bloomTex);	// bind the first threshold pass to the sampler
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, downsampledTex[i - 1]);	// bind the first threshold pass to the sampler
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
		bloomDownsample->unbind();
		first = false;
	}
}

void Bloom::Upsample(GLuint quad_vao, int width, int height)
{
	// start off by rendering a single downsample into FBO2
	for (int i = 0; i < num_downsamples; i++)
	{
		int j = num_downsamples - i - 1;	// index into lower res upsamples first
		glBindFramebuffer(GL_FRAMEBUFFER, upsampledFBOs[j]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, upsampledTex[j], 0);	// attach tex to framebuffer
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, width / (2 * (j + 2)), height / (2 * (j + 2)));	// set viewport to texture size (hopefully)
		bloomUpsample->bind();
		glBindVertexArray(quad_vao);
		glActiveTexture(GL_TEXTURE0);
		if (j == 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, downsampledTex[0]);	// bind lowres

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, bloomTex);	// bind currentRes
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, downsampledTex[j]);	// bind lowres

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, downsampledTex[j - 1]);	// bind currentRes
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
		bloomUpsample->unbind();
	}
}

void Bloom::RenderBloom(GLuint quad_vao, GLuint screenTexture, int width, int height)
{
	// clear all framebuffers of color data
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);

	// glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// glClear(GL_DEPTH_BUFFER_BIT);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width / 2, height / 2);
	// extract out threshold values into bloomTex (bloomFBO)
	bloomThresholdProg->bind();
	glBindVertexArray(quad_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	bloomThresholdProg->unbind();
	// downsample image 5 times:
	DownSample(quad_vao, width, height);

	// upscale image into bloomTex (bloomFBO)
	Upsample(quad_vao, width, height);
}