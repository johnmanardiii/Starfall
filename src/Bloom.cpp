#include "Bloom.h"
#include <cmath>
#include <iostream>
using namespace std; 

// Intitializes all shaders from the files. Only needs to be called once at
// the beginning.
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
	glUniform1i(TexLocation, 1);
}

// sets up all framebuffers required for rendering bloom.
void Bloom::InitializeFramebuffers(int width, int height)
{
	// calculate number of downsamples needed and allocate that much memory for
	// FBO and Tex arrays
	//num_downsamples = int(min(log2(width), log2(height))) - 1;

	//downsampleFBOs = new GLuint[num_downsamples];
	//downsampledTex = new GLuint[num_downsamples];
	//upsampledFBOs = new GLuint[num_downsamples];
	//upsampledTex = new GLuint[num_downsamples];

	int scaleDownFactor = 1;
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
		scaleDownFactor *= 2;
		glBindFramebuffer(GL_FRAMEBUFFER, downsampleFBOs[i]);	// threshold
		glBindTexture(GL_TEXTURE_2D, downsampledTex[i]);
		// make the texture a floating point texture for HDR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width / scaleDownFactor, height / scaleDownFactor, 0, GL_RGBA, GL_FLOAT, NULL);
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
	scaleDownFactor = 1;
	// generate upsample textures and FBOs
	glGenFramebuffers(num_downsamples, upsampledFBOs);
	// Create offscreen color texture and bind it to framebuffer
	glGenTextures(num_downsamples, upsampledTex);
	for (int i = 0; i < num_downsamples; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, upsampledFBOs[i]);	// threshold
		glBindTexture(GL_TEXTURE_2D, upsampledTex[i]);
		// make the texture a floating point texture for HDR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width / scaleDownFactor, height / scaleDownFactor, 0, GL_RGBA, GL_FLOAT, NULL);
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
		scaleDownFactor *= 2;
	}
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bloom::OnResizeWindow()
{
	DeleteFBOTex();
	InitializeFramebuffers(postProcessing->GetWidth(), postProcessing->GetHeight());
}

Bloom::Bloom(PostProcessing* pp)
{
	this->postProcessing = pp;

	// initialize framebuffers + textures
	InitializeFramebuffers(postProcessing->GetWidth(), postProcessing->GetHeight());
	// intialize shaders
	InitializeShaders();
}

void Bloom::DeleteFBOTex()
{
	glDeleteFramebuffers(1, &bloomFBO);
	glDeleteFramebuffers(num_downsamples, downsampleFBOs);
	glDeleteFramebuffers(num_downsamples, upsampledFBOs);

	glDeleteTextures(1, &bloomTex);
	glDeleteTextures(num_downsamples, downsampledTex);
	glDeleteTextures(num_downsamples, upsampledTex);

	//delete[] downsampleFBOs;
	//delete[] downsampledTex;
	//delete[] upsampledFBOs;
	//delete[] upsampledTex;
}

Bloom::~Bloom()
{
	DeleteFBOTex();
}

// Renders the threshold scene into lower and lower resolutions, using 13-box downsample to average out the pixels
// as the resolution is halved each pass.
void Bloom::DownSample()
{
	bool first = true;
	int scaleDownFactor = 1;
	// start off by rendering a single downsample into FBO2
	for (int i = 0; i < num_downsamples; i++)
	{
		scaleDownFactor *= 2;
		glBindFramebuffer(GL_FRAMEBUFFER, downsampleFBOs[i]);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, postProcessing->GetWidth() / scaleDownFactor, postProcessing->GetHeight() / scaleDownFactor);	// set viewport to texture size (hopefully)
		bloomDownsample->bind();
		glBindVertexArray(postProcessing->GetQuadVAO());
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

// Combines the last-lowest resolution with the next-highest resolution by upsampling the lower resolution image
// and adding it to the higher resolution image. This is where exponential falloff occurs and I am not sure
// if it should because the upsample uses a tent filter but it doesn't seem to do enough.
void Bloom::Upsample()
{
	bool first = true;
	int scaleDownFactor = pow(2, num_downsamples);
	bloomUpsample->bind();
	// start off by rendering a single downsample into FBO2
	for (int i = 0; i < num_downsamples ; i++)
	{
		scaleDownFactor /= 2;
		int j = num_downsamples - i - 1;	// index into lower res upsamples first
		glBindFramebuffer(GL_FRAMEBUFFER, upsampledFBOs[j]);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, postProcessing->GetWidth() /  (scaleDownFactor), postProcessing->GetHeight() / (scaleDownFactor));	// set viewport to texture size (hopefully)
		glBindVertexArray(postProcessing->GetQuadVAO());
		glActiveTexture(GL_TEXTURE0);
		if (j == 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, upsampledTex[1]);	// bind lowres (highest res upsample (width/2, height/2))

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, bloomTex);	// bind (full-res threshold image)
		}
		if (first)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, downsampledTex[j]);	// bind lowest res downsample

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, downsampledTex[j - 1]);	// bind next lowest res downsample
			first = false;
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, upsampledTex[j + 1]);	// bind lowres upsample [AFTER THE FIRST, THIS NEEDS TO BE THE UPSAMPLED TEX!!!]

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, downsampledTex[j - 1]);	// bind currentRes downsampled image
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	bloomUpsample->unbind();
}

// Renders all bloom using the PostProcessing BaseTex.
// Result of sampling threshold values found in upsampledTex[0]
void Bloom::RenderBloom()
{
	// clear all framebuffers of color data
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, postProcessing->GetWidth(), postProcessing->GetHeight());
	// extract out threshold values into bloomTex (bloomFBO)
	bloomThresholdProg->bind();
	glBindVertexArray(postProcessing->GetQuadVAO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, postProcessing->GetBaseTex());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	bloomThresholdProg->unbind();
	// downsample image 5 times:
	DownSample();
	// upscale image into bloomTex (bloomFBO)
	Upsample();
}