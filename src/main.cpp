/* Hello triangle lab - intro OGL
Z. Wood + S. Sueda
*/

#include <iostream>
#include <glad/glad.h>
#include <chrono>

#include "Application.h"
#include "WindowManager.h"

using namespace std;
using namespace glm;

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main(int argc, char *argv[])
{
	srand(time(0));
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->Init(SCREEN_WIDTH, SCREEN_HEIGHT);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->Init(resourceDir);

	// Loop until the user closes the window.
	auto lastTime = chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(windowManager->getHandle()))
	{

		auto nextLastTime = chrono::high_resolution_clock::now();
		float deltaTime = // the time that has elapsed between the start of previous frame and start of this frame
				chrono::duration_cast<std::chrono::microseconds>(
						chrono::high_resolution_clock::now() - lastTime)
						.count();
		// convert microseconds (weird) to seconds (less weird)
		deltaTime *= 0.000001f;

		// reset lastTime so that we can calculate the deltaTime
		// on the next frame
		lastTime = nextLastTime;

		application->render(deltaTime);

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	application->audioEngine.Cleanup();
	windowManager->shutdown();
	return 0;
}
