#include "Application.h"
void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
  
	//player movement
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		componentManager.GetPlayer().SetInput(W, true);
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		componentManager.GetPlayer().SetInput(A, true);
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		componentManager.GetPlayer().SetInput(S, true);
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		componentManager.GetPlayer().SetInput(D, true);
	}

	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		componentManager.GetPlayer().SetInput(W, false);
	}

	if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		componentManager.GetPlayer().SetInput(A, false);
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		componentManager.GetPlayer().SetInput(S, false);
	}

	if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		componentManager.GetPlayer().SetInput(D, false);
	}

	// Falling toggle
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
		componentManager.GetPlayer().SetInput(LSHIFT, true);
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
		componentManager.GetPlayer().SetInput(LSHIFT, false);
	}
}

void Application::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	double posX, posY;

	if (action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &posX, &posY);
		cout << "Pos X " << posX << " Pos Y " << posY << endl;
	}
}

void Application::mouseMovementCallback(GLFWwindow* window, double posX, double posY)
{
	
}

void Application::InitTerrain() {
	TerrainS terrain;
	terrain.numVerts = MESHSIZE * MESHSIZE * 6;
	//generate the VAO
	glGenVertexArrays(1, &terrain.VAOId);
	glBindVertexArray(terrain.VAOId);

	//generate vertex buffer to hand off to OGL
	GLuint terrainPosBuf;
	glGenBuffers(1, &terrainPosBuf);
	glBindBuffer(GL_ARRAY_BUFFER, terrainPosBuf);
	vec3* vertices = (vec3*)malloc(MESHSIZE * MESHSIZE * 4 * sizeof(vec3));
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
	vec2* tex = (vec2*)malloc(MESHSIZE * MESHSIZE * 4 * sizeof(vec2));
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
	GLushort* elements = (GLushort*)malloc(MESHSIZE * MESHSIZE * 6 * sizeof(GLushort));
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

void Application::InitShaderManager(const std::string& resourceDirectory)
{
	shaderManager = ShaderManager::GetInstance();
	GLuint tex;
	int width, height, channels;
	
	//this lambda does a general loadTexture. Can add more parameters to change its behavior if needed.
	auto loadTexture = [this, resourceDirectory, &tex, &width, &height, &channels] //captures
	(string filename, string managerIdentifer) { //parameters
		char filepath[1000];
		string str = resourceDirectory + filename;
		strcpy(filepath, str.c_str());
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		shaderManager.SetTexture(managerIdentifer, tex);
	};

	// load all textures
	loadTexture("/cat.jpg", "Cat");
	loadTexture("/LUNA/LUNA_test_tex.png", "Luna");
	loadTexture("/grass.jpg", "Grass");
	loadTexture("/alpha.png", "Alpha");
	loadTexture("/noiseTex.png", "noiseTex");
	loadTexture("/rainbow.jpg", "Rainbow");

	// used on Luna
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

	//used for particle effects on star fragments
	auto partProg = make_shared<Program>();
	partProg->setVerbose(true);
	partProg->setShaderNames(
		resourceDirectory + "/particle_vert.glsl",
		resourceDirectory + "/particle_frag.glsl");
	partProg->Init();
	partProg->addUniform("P");
	partProg->addUniform("M");
	partProg->addUniform("V");
	partProg->addUniform("totalTime");
	partProg->addUniform("centerPos");
	partProg->addUniform("alphaTexture");
	partProg->addUniform("alphaMult");
	partProg->addUniform("rainbowTexture");
	partProg->addAttribute("pColor");
	partProg->addAttribute("pNormal");
	partProg->addAttribute("pRotation");

	GLuint PartLocation0 = glGetUniformLocation(partProg->pid, "particle");
	GLuint PartLocation1 = glGetUniformLocation(partProg->pid, "particle");
	glUseProgram(partProg->pid);
	glUniform1i(PartLocation0, 0);
	glUniform1i(PartLocation1, 0);

	shaderManager.SetShader("particle", partProg);

	//used for star fragments
	auto starProg = make_shared<Program>();
	starProg->setVerbose(true);
	starProg->setShaderNames(
		resourceDirectory + "/star_vert.glsl",
		resourceDirectory + "/star_frag.glsl");
	starProg->Init();
	starProg->addUniform("P");
	starProg->addUniform("M");
	starProg->addUniform("V");
	starProg->addUniform("starTexture");
	starProg->addUniform("totalTime");
	starProg->addUniform("centerPos");
	starProg->addUniform("campos");
	starProg->addAttribute("vertPos");
	starProg->addAttribute("vertNor");
	starProg->addAttribute("vertTex");

	GLuint StarLocation = glGetUniformLocation(starProg->pid, "Star");
	glUseProgram(starProg->pid);
	glUniform1i(StarLocation, 0);

	shaderManager.SetShader("Star", starProg);

	
	//used for explosion
	auto explosionProg = make_shared<Program>();
	explosionProg->setVerbose(true);
	explosionProg->setShaderNames(
		resourceDirectory + "/explosion_vert.glsl",
		resourceDirectory + "/explosion_frag.glsl");
	explosionProg->Init();
	explosionProg->addUniform("P");
	explosionProg->addUniform("M");
	explosionProg->addUniform("V");
	explosionProg->addUniform("starTexture");
	explosionProg->addUniform("totalTime");
	explosionProg->addUniform("centerPos");
	explosionProg->addAttribute("vertPos");
	explosionProg->addAttribute("vertNor");
	explosionProg->addAttribute("vertTex");

	GLuint ExplosionLocation = glGetUniformLocation(explosionProg->pid, "Explosion");
	glUseProgram(explosionProg->pid);
	glUniform1i(ExplosionLocation, 0);

	shaderManager.SetShader("Explosion", explosionProg);
	


	// Terrain Shader
	auto heightProg = make_shared<Program>();
	heightProg->setVerbose(true);
	heightProg->setShaderNames(resourceDirectory + "/height_vertex.glsl", resourceDirectory + "/height_frag.glsl");
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
	vector<string> filenames = { "sphere", "Star Bit", "icoSphere", "LUNA/luna_arm",
		"LUNA/luna_arm2", "LUNA/luna_body", "LUNA/luna_head" };
	vec3 explosionScaleFactor = vec3(60.0f);
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
			if (filenames[i] == "icoSphere") {
				shape->scale(explosionScaleFactor);
			}
			shape->computeNormals();
			shape->Init();

			shaderManager.SetModel(filenames[i], shape);
		}
	}

	InitTerrain();
}

void Application::Init(std::string resourceDirectory)
{
	GLSL::checkVersion();
	// lock the mouse cursor
	glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set background color.
	// glClearColor(.12f, .34f, .56f, 1.0f);
	// Enable z-buffer test.

	CHECKED_GL_CALL(glEnable(GL_DEPTH_TEST));
	CHECKED_GL_CALL(glEnable(GL_BLEND));
	CHECKED_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glPointSize(5.0f);
	InitShaderManager(resourceDirectory);
	// do ComponentManager's init here
	componentManager.Init(resourceDirectory);
	audioEngine.Init(resourceDirectory);
	audioEngine.Play("tomorrow.mp3");

	postProcessing = make_shared<PostProcessing>(windowManager, &componentManager.GetCamera());
}

void Application::render(float frameTime)
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


	// clear all framebuffers
	postProcessing->SetUpFrameBuffers();
	componentManager.UpdateComponents(frameTime, width, height);

	// render post-processing
	postProcessing->RenderPostProcessing();
}
