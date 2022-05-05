#pragma once

#include "GLSL.h"
#include "ComponentManager.h"
#include "Event.h"
#include "EventManager.h"
#include "Audio.h"
#include "PostProcessing.h"

#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
// For Input enum
#include "PlayerMovement.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"


class Application : public EventCallbacks
{
public:
	// the various managers that make up the application.
	ComponentManager componentManager;
	ShaderManager& shaderManager = ShaderManager::GetInstance();
	WindowManager* windowManager = nullptr;
	AudioEngine audioEngine;
	shared_ptr<PostProcessing> postProcessing;
	
	//callbacks
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseCallback(GLFWwindow* window, int button, int action, int mods);
	void mouseMovementCallback(GLFWwindow* window, double posX, double posY);
	void resizeCallback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

#define MESHSIZE 100
	//initialization
	void InitTerrain();
	void InitShaderManager(const std::string& resourceDirectory);
	void Init(std::string resourceDirectory);
	
	//render loop
	void render(float frameTime);
};