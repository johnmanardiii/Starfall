#include "PostProcessing.h"
#include <string>
#include <iostream>

using namespace std;

template<typename T>
T exponential_growth(T actual, T goal, float factor, float frametime)
{
	return actual + (goal - actual) * factor * frametime;
}

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
#ifdef __APPLE__
    std::string resourceDir = "../../resources";
#else
	std::string resourceDir = "../resources";
#endif
	simple_prog = make_shared<Program>();
	simple_prog->setVerbose(true);
	simple_prog->setShaderNames(resourceDir + "/post_vert.glsl", resourceDir + "/post_simple_frag.glsl");
	simple_prog->Init();
	simple_prog->addAttribute("vertPos");
	simple_prog->addAttribute("texCoord");

	GLuint TexLocation = glGetUniformLocation(simple_prog->pid, "screenTexture");
	glUseProgram(simple_prog->pid);
	glUniform1i(TexLocation, 0);
	TexLocation = glGetUniformLocation(simple_prog->pid, "bloomTexture");
	glUniform1i(TexLocation, 1);
}

void PostProcessing::InitializeFramebuffers()
{
	// initialize default size of framebuffer textures
	// Get current frame buffer size.
	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
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
	// make the texture a floating point texture for HDR
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, base_color, 0);


	// make a texture for the depth buffer for rendering motion blur and other effects.
	glGenTextures(1, &base_depth);
	glBindTexture(GL_TEXTURE_2D, base_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, base_depth, 0);

	// check if framebuffer is set up properly:
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	// Clear default framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PostProcessing::PostProcessing(WindowManager* wm, Camera* cam)
{
	windowManager = wm;
	camera = cam;
	InitializeQuad();
	InitializeShaders();
	InitializeFramebuffers();

	// initialize Bloom rendering object
	bloom = make_shared<Bloom>(this);
	mb = make_shared<MotionBlur>(this);
	rb = make_shared<RadialBlur>(this);
}

void PostProcessing::DeleteTexturesFramebuffers()
{
	glDeleteFramebuffers(1, &base_fbo);
	glDeleteTextures(1, &base_color);
	glDeleteTextures(1, &base_depth);
}

PostProcessing::~PostProcessing()
{
	glDeleteBuffers(1, &quad_vbo);
	glDeleteVertexArrays(1, &quad_vao);
}

// Clears the Main frame buffer and binds it as an active framebuffer for component manager.
// This method should be called before every frame.
void PostProcessing::SetUpFrameBuffers()
{
	// check if width/height has changed
	int currWidth, currHeight;
	glfwGetFramebufferSize(windowManager->getHandle(), &currWidth, &currHeight);
	if (currWidth != width || currHeight != height)
	{
		width = currWidth;
		height = currHeight;
		DeleteTexturesFramebuffers();
		InitializeFramebuffers();
		// tell bloom and motion blur that resolution has changed
		// and the framebuffers and viewports need to be
		// re-initialized.
		bloom->OnResizeWindow();
		mb->OnResizeWindow();
		rb->OnResizeWindow();
	}

	// clear the base framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, base_fbo);
	glClearColor(0.0, 0.0, 0.0, 1.0);	// clear color for background of scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// set render target to default offscreen framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, base_fbo);
	glEnable(GL_DEPTH_TEST);

	//set culling back to normal
	glCullFace(GL_BACK);
}

void PostProcessing::SetLastProcessedScreen(GLuint tex)
{
	last_processed_screen = tex;
}

// Renders all effects to the screen using the main framebuffer.
void PostProcessing::RenderPostProcessing(float frameTime, float goalBlur)
{
	last_processed_screen = base_color;
	glDisable(GL_DEPTH_TEST);
	// Generate the mipmaps for the rendered scene
	glBindTexture(GL_TEXTURE_2D, base_color);
	glGenerateMipmap(GL_TEXTURE_2D);

	// mb->RenderMotionBlur(camera);
	currentBlur = exponential_growth(currentBlur, goalBlur, .02f * 60.0f, frameTime);
	rb->SetBlurAmount(currentBlur);
	rb->RenderRadialBlur();	// uncomment this if you want radial blur for speed.

	// generate bloom
	// glViewport(0, 0, width, height);
	bloom->RenderBloom();
	glViewport(0, 0, width, height);

	// clear the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// bind default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	simple_prog->bind();
	glBindVertexArray(quad_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GetLastProcessedScreen());
	// bind in bloom texture and additive blend

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloom->GetBloomTex());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	simple_prog->unbind();

	//glEnable(GL_DEPTH_TEST);
}
