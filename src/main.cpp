/* Hello triangle lab - intro OGL
Z. Wood + S. Sueda
*/

#include <iostream>
#include <glad/glad.h>
#include <chrono>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"
#include "ComponentManager.h"
#include "Event.h"
#include "EventManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

/* Global data associated with triangle geometry - this will likely vary
in later programs - so is left explicit for now  */
static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 
        1.0f, 1.0f, 0.0f
    };

/* A big global wrapper for all our data */
class Application : public EventCallbacks {

public:
	// the component manager.
	ComponentManager componentManager;

	WindowManager * windowManager = nullptr;

	typedef struct Ground{
        GLuint VertexArrayID;
        GLuint buffObj;
        GLuint norBuffObj;
        GLuint texBuffObj;
        GLuint indexBuffObj;
        int giboLen;
    }Ground;
    Ground ground;

	// Our shader program
	std::shared_ptr<Program> prog;

	// Shape to be used (from  file) - modify to support multiple
	shared_ptr<Shape> mesh;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint vertexBufferID;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
		}
	}

	void mouseMovementCallback(GLFWwindow* window, double posX, double posY) {
		Camera& cam = componentManager.GetCamera();
		cam.Update(posX, posY);
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		//do ComponentManager's init here
		componentManager.Init(resourceDirectory);
	}
	
	void drawGround(std::shared_ptr<Program> curS) {
	    curS->bind();
	    glBindVertexArray(ground.VertexArrayID);
	    //draw the ground plane
	    mat4 Model = glm::scale(glm::mat4(1.0f), vec3(0.5,1,0.5));
		mat4 View = componentManager.GetCamera().GetView();
		componentManager.GetCamera().CalcPerspective(windowManager);
		mat4 Perspective = componentManager.GetCamera().GetPerspective();
	    glUniformMatrix4fv(curS->getUniform("M"), 1, GL_FALSE, value_ptr(Model));
		glUniformMatrix4fv(curS->getUniform("V"), 1, GL_FALSE, value_ptr(View));
		glUniformMatrix4fv(curS->getUniform("P"), 1, GL_FALSE, value_ptr(Perspective));

	    glEnableVertexAttribArray(0);
	    glBindBuffer(GL_ARRAY_BUFFER, ground.buffObj);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	    glEnableVertexAttribArray(1);
	    glBindBuffer(GL_ARRAY_BUFFER, ground.norBuffObj);
	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	    glEnableVertexAttribArray(2);
	    glBindBuffer(GL_ARRAY_BUFFER, ground.texBuffObj);
	    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	    // draw!
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ground.indexBuffObj);
	    glDrawElements(GL_TRIANGLES, ground.giboLen, GL_UNSIGNED_SHORT, 0);
	
	    glDisableVertexAttribArray(0);
	    glDisableVertexAttribArray(1);
	    glDisableVertexAttribArray(2);
	    curS->unbind();
	}
	
	void initGround(float g_groundY) {
	
	    float g_groundSize = 20;
	    
	    // A x-z plane at y = g_groundY of dimension [-g_groundSize, g_groundSize]^2
	    float GrndPos[] = {
	        -g_groundSize, g_groundY, -g_groundSize,
	        -g_groundSize, g_groundY,  g_groundSize,
	        g_groundSize, g_groundY,  g_groundSize,
	        g_groundSize, g_groundY, -g_groundSize
	    };
	
	    float GrndNorm[] = {
	        0, 1, 0,
	        0, 1, 0,
	        0, 1, 0,
	        0, 1, 0,
	        0, 1, 0,
	        0, 1, 0
	    };
	
	    static GLfloat GrndTex[] = {
	        0, 0, // back
	        0, 1,
	        1, 1,
	        1, 0 };
	 
	    unsigned short idx[] = {0, 1, 2, 0, 2, 3};
	
	    //generate the ground VAO
	    glGenVertexArrays(1, &ground.VertexArrayID);
	    glBindVertexArray(ground.VertexArrayID);
	
	    ground.giboLen = 6;
	    glGenBuffers(1, &ground.buffObj);
	    glBindBuffer(GL_ARRAY_BUFFER, ground.buffObj);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);
	
	    glGenBuffers(1, &ground.norBuffObj);
	    glBindBuffer(GL_ARRAY_BUFFER, ground.norBuffObj);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndNorm), GrndNorm, GL_STATIC_DRAW);
	
	    glGenBuffers(1, &ground.texBuffObj);
	    glBindBuffer(GL_ARRAY_BUFFER, ground.texBuffObj);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);
	
	    glGenBuffers(1, &ground.indexBuffObj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ground.indexBuffObj);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
	}

	void initGeom(const std::string& resourceDirectory)
	{
		prog = make_shared<Program>();
		prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		prog->Init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");

		prog->addAttribute("vertPos");
		
		initGround(0.0);
		
		//generate the VAO
		/*glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		//generate vertex buffer to hand off to OGL
		glGenBuffers(1, &vertexBufferID);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		//actually memcopy the data - only do this once
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
		*/
	}

	void render(float frameTime)
	{
		//local modelview matrix use this for later labs
		
		auto M = make_shared<MatrixStack>();
		auto V = make_shared<MatrixStack>();
		auto P = make_shared<MatrixStack>();

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use the local matrices for lab 4
		float aspect = width/(float)height;
		//createPerspectiveMat(P, 70.0f, aspect, 0.1, 100.0f);	
		
		componentManager.UpdateComponents(frameTime);
		V->pushMatrix();
		V->multMatrix(componentManager.GetCamera().GetView());
		// Draw mesh using GLSL.
		
		
		drawGround(prog);

	}	
};

int main(int argc, char *argv[])
{
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
	windowManager->Init(640, 480);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->Init(resourceDir);
	application->initGeom(resourceDir);

	// Loop until the user closes the window.
	auto lastTime = chrono::high_resolution_clock::now();
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{

		auto nextLastTime = chrono::high_resolution_clock::now();
		float deltaTime = //the time that has elapsed between the start of previous frame and start of this frame
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
	windowManager->shutdown();
	return 0;
}
