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
#include "Audio.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"

using namespace std;
using namespace glm;

/* Global data associated with triangle geometry - this will likely vary
in later programs - so is left explicit for now  */
static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f};

/* A big global wrapper for all our data */
class Application : public EventCallbacks
{

public:
	// the component manager.
	ComponentManager componentManager;
	ShaderManager& shaderManager = ShaderManager::GetInstance();
	WindowManager *windowManager = nullptr;
	AudioEngine audioEngine;
	typedef struct Ground
	{
		GLuint VertexArrayID;
		GLuint buffObj;
		GLuint norBuffObj;
		GLuint texBuffObj;
		GLuint indexBuffObj;
		int giboLen;
	} Ground;
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
		if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
		{
			componentManager.GetCamera().AdjustMovementSpeed(0.5);
		}
		if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
		{
			componentManager.GetCamera().AdjustMovementSpeed(2);
		}
        //camera movement
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
                componentManager.GetPlayer().inputBuffer[0] = true;
                //eyePos -= movementSensitivity * w;
            }
            
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            componentManager.GetPlayer().inputBuffer[1] = true;
            //eyePos += movementSensitivity * u;
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {
            componentManager.GetPlayer().inputBuffer[2] = true;
            //eyePos += movementSensitivity * w;
        }

        if (key == GLFW_KEY_D && action == GLFW_PRESS) {
            componentManager.GetPlayer().inputBuffer[3] = true;
            //eyePos -= movementSensitivity * u;
        }

        if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
            componentManager.GetPlayer().inputBuffer[0] = false;
        }

        if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
            componentManager.GetPlayer().inputBuffer[1] = false;
        }
        if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
            componentManager.GetPlayer().inputBuffer[2] = false;
        }

        if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
            componentManager.GetPlayer().inputBuffer[3] = false;
        }
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			cout << "Pos X " << posX << " Pos Y " << posY << endl;
		}
	}

	void mouseMovementCallback(GLFWwindow *window, double posX, double posY)
	{
		Camera &cam = componentManager.GetCamera();
		cam.Update(posX, posY);
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

#define MESHSIZE 100
	void InitTerrain() {
		TerrainS terrain;
		terrain.numVerts = MESHSIZE * MESHSIZE * 6;
		//generate the VAO
		glGenVertexArrays(1, &terrain.VAOId);
		glBindVertexArray(terrain.VAOId);

		//generate vertex buffer to hand off to OGL
		GLuint terrainPosBuf;
		glGenBuffers(1, &terrainPosBuf);
		glBindBuffer(GL_ARRAY_BUFFER, terrainPosBuf);
		vec3 *vertices = (vec3*) malloc(MESHSIZE * MESHSIZE * 4 * sizeof(vec3));
		for (int x = 0; x < MESHSIZE; x++)
			for (int z = 0; z < MESHSIZE; z++)
			{
				vertices[x * 4 + z * MESHSIZE * 4 + 0] = vec3(0.0, 0.0, 0.0) + vec3(x, 0, z);
				vertices[x * 4 + z * MESHSIZE * 4 + 1] = vec3(1.0, 0.0, 0.0) + vec3(x, 0, z);
				vertices[x * 4 + z * MESHSIZE * 4 + 2] = vec3(1.0, 0.0, 1.0) + vec3(x, 0, z);
				vertices[x * 4 + z * MESHSIZE * 4 + 3] = vec3(0.0, 0.0, 1.0) + vec3(x, 0, z);
			}
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * MESHSIZE * MESHSIZE * 4, vertices, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		free(vertices);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//tex coords
		float t = 1. / 100;
		vec2 *tex = (vec2*) malloc(MESHSIZE * MESHSIZE * 4 * sizeof(vec2));
		for (int x = 0; x < MESHSIZE; x++)
			for (int y = 0; y < MESHSIZE; y++)
			{
				tex[x * 4 + y * MESHSIZE * 4 + 0] = vec2(0.0, 0.0) + vec2(x, y) * t;
				tex[x * 4 + y * MESHSIZE * 4 + 1] = vec2(t, 0.0) + vec2(x, y) * t;
				tex[x * 4 + y * MESHSIZE * 4 + 2] = vec2(t, t) + vec2(x, y) * t;
				tex[x * 4 + y * MESHSIZE * 4 + 3] = vec2(0.0, t) + vec2(x, y) * t;
			}

		GLuint terrainTexBuf;
		glGenBuffers(1, &terrainTexBuf);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, terrainTexBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * MESHSIZE * MESHSIZE * 4, tex, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		free(tex);

		glGenBuffers(1, &terrain.IndexBuff);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain.IndexBuff);
		GLushort *elements = (GLushort*) malloc(MESHSIZE * MESHSIZE * 6 * sizeof(GLushort));
		int ind = 0;
		for (int i = 0; i < MESHSIZE * MESHSIZE * 6; i += 6, ind += 4)
		{
			elements[i + 0] = ind + 0;
			elements[i + 1] = ind + 1;
			elements[i + 2] = ind + 2;
			elements[i + 3] = ind + 0;
			elements[i + 4] = ind + 2;
			elements[i + 5] = ind + 3;
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * MESHSIZE * MESHSIZE * 6, elements, GL_STATIC_DRAW);
		glBindVertexArray(0);
		free(elements);
		shaderManager.Terrain = terrain;
	}

	void InitShaderManager(const std::string &resourceDirectory)
	{
		shaderManager = ShaderManager::GetInstance();
		GLuint tex;
		int width, height, channels;
		char filepath[1000];

		// load cat texture
		string str = resourceDirectory + "/cat.jpg";
		strcpy(filepath, str.c_str());
		unsigned char *data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		shaderManager.SetTexture("Cat", tex);

		str = resourceDirectory + "/LUNA/LUNA_test_tex.png";
		strcpy(filepath, str.c_str());
		data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		shaderManager.SetTexture("Luna", tex);

		// load saturn texture
		str = resourceDirectory + "/grass.jpg";
		strcpy(filepath, str.c_str());
		data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		shaderManager.SetTexture("Grass", tex);
    
		// load alpha particle texture
		str = resourceDirectory + "/alpha.bmp";
    
    strcpy(filepath, str.c_str());
		data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
    
    shaderManager.SetTexture("Alpha", tex);
    
    
		// load noise texture
		str = resourceDirectory + "/noiseTex.png";

		strcpy(filepath, str.c_str());
		data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		shaderManager.SetTexture("noiseTex", tex);


		auto prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/tex_vert.glsl", resourceDirectory + "/tex_frag.glsl");
		prog->Init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("flashAmt");
		prog->addUniform("flashCol");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");

		GLuint TexLocation = glGetUniformLocation(prog->pid, "tex");
		glUseProgram(prog->pid);
		glUniform1i(TexLocation, 0);

		shaderManager.SetShader("Texture", prog);

		auto partProg = make_shared<Program>();
		partProg->setVerbose(true);
		partProg->setShaderNames(
			resourceDirectory + "/particle_vert.glsl",
			resourceDirectory + "/particle_frag.glsl");
		partProg->Init();
		partProg->addUniform("P");
		partProg->addUniform("M");
		partProg->addUniform("V");
		partProg->addUniform("alphaTexture");
		partProg->addAttribute("vertPos");
		partProg->addAttribute("pColor");
    
    GLuint PartLocation = glGetUniformLocation(partProg->pid, "particle");
		glUseProgram(partProg->pid);
		glUniform1i(PartLocation, 0);

		shaderManager.SetShader("particle", partProg);

		// Terrain Shader
		auto heightProg = make_shared<Program>();
		heightProg->setVerbose(true);
		heightProg->setShaderNames(resourceDirectory + "/height_vertex.glsl", resourceDirectory + "/height_frag.glsl", resourceDirectory + "/height_geom.glsl");
		if (!heightProg->Init())

		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}

		heightProg->addUniform("P");
		heightProg->addUniform("V");
		heightProg->addUniform("M");
		heightProg->addUniform("camoff");
		heightProg->addUniform("campos");
		heightProg->addUniform("lightDir");
		heightProg->addAttribute("vertPos");
		heightProg->addAttribute("vertTex");
		assert(glGetError() == GL_NO_ERROR);

		TexLocation = glGetUniformLocation(heightProg->pid, "tex");
		GLuint TexLocation2 = glGetUniformLocation(heightProg->pid, "tex2");
		GLuint TexLocation3 = glGetUniformLocation(heightProg->pid, "noiseTex");
		glUseProgram(heightProg->pid);
		glUniform1i(TexLocation, 0);
		glUniform1i(TexLocation2, 1);
		glUniform1i(TexLocation3, 2);

		assert(glGetError() == GL_NO_ERROR);

		shaderManager.SetShader("Height", heightProg);

		//the obj files you want to load. Add more to read them all.
		vector<string> filenames = { "sphere", "suzanne", "LUNA/luna_arm", 
			"LUNA/luna_arm2", "LUNA/luna_body", "LUNA/luna_head"};
		//where the data is held
		vector<vector<tinyobj::shape_t>> TOshapes(filenames.size());
		vector<tinyobj::material_t> objMaterials; //not using for now.
		bool rc = false;
		string errStr;
		for (size_t i = 0; i < filenames.size(); ++i) {
			rc = tinyobj::LoadObj(TOshapes[i], objMaterials, errStr, (resourceDirectory + "/" + filenames[i] + ".obj").c_str());
			if (!rc) {
				cerr << errStr << endl;
				exit(EXIT_FAILURE);
			}
			else {
				//some obj files have multiple shapes, read just the first one for now.
				shared_ptr<Shape> shape = make_shared<Shape>();
				shape->createShape(TOshapes[i][0]); //the first (0'th) shape read in of the i'th file.
				shape->measure();
				shape->Init();
				
				shaderManager.SetModel(filenames[i], shape);
			}
		}

		InitTerrain();
	}

	void Init(std::string resourceDirectory)
	{
		GLSL::checkVersion();
		// lock the mouse cursor
		glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		
		CHECKED_GL_CALL(glEnable(GL_DEPTH_TEST));
		CHECKED_GL_CALL(glEnable(GL_BLEND));
		CHECKED_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glPointSize(10.0f);
		InitShaderManager(resourceDirectory);
		// do ComponentManager's init here
		componentManager.Init(resourceDirectory);
		audioEngine.Init(resourceDirectory);
		audioEngine.PlaySoundOnce("tomorrow.mp3");
	}

	void drawGround(std::shared_ptr<Program> curS, int width, int height)
	{
		curS->bind();
		glBindVertexArray(ground.VertexArrayID);
		// draw the ground plane
		mat4 Model = glm::scale(glm::mat4(1.0f), vec3(0.5, 1, 0.5));
		mat4 View = componentManager.GetCamera().GetView();
		componentManager.GetCamera().CalcPerspective(width, height);
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

		glUniform1f(curS->getUniform("flashAmt"), 0.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shaderManager.GetTexture("Grass"));

		// draw!
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ground.indexBuffObj);
		glDrawElements(GL_TRIANGLES, ground.giboLen, GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		curS->unbind();
	}

	void initGround(float g_groundY)
	{

		float g_groundSize = 100;

		// A x-z plane at y = g_groundY of dimension [-g_groundSize, g_groundSize]^2
		float GrndPos[] = {
				-g_groundSize, g_groundY, -g_groundSize,
				-g_groundSize, g_groundY, g_groundSize,
				g_groundSize, g_groundY, g_groundSize,
				g_groundSize, g_groundY, -g_groundSize};

		float GrndNorm[] = {
				0, 1, 0,
				0, 1, 0,
				0, 1, 0,
				0, 1, 0,
				0, 1, 0,
				0, 1, 0};

		static GLfloat GrndTex[] = {
				0, 0, // back
				0, 1,
				1, 1,
				1, 0};

		unsigned short idx[] = {0, 1, 2, 0, 2, 3};

		// generate the ground VAO
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

	void initGeom(const std::string &resourceDirectory)
	{
		prog = make_shared<Program>();
		prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		prog->Init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");

		prog->addAttribute("vertPos");

		initGround(0.0);

		// generate the VAO
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
		// local modelview matrix use this for later labs

		auto M = make_shared<MatrixStack>();
		auto V = make_shared<MatrixStack>();
		auto P = make_shared<MatrixStack>();

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		componentManager.UpdateComponents(frameTime, width, height);
		
		// Draw mesh using GLSL.
		//drawGround(shaderManager.GetShader("Texture"), width, height);
	}
};

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
	windowManager->Init(1920, 1200);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->Init(resourceDir);
	application->initGeom(resourceDir);

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
