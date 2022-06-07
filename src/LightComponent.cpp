#include "LightComponent.h"
#include "imgui/imgui.h"
#include "ComponentManager.h"

void LightComponent::Update(float frameTime, ComponentManager* compMan)
{
	GameState* gameState = compMan->GetGameState();
	if (gameState->IsGameEnded()) return;
	if (gameState->timeLeft > 0)
	{ 
		gameState->timeLeft -= frameTime;
		sunRotation = glm::mix(endSunRotation, startSunRotation, gameState->timeLeft / gameState->startTime);
		cout << "Time Left Until Moon Sets: " << gameState->timeLeft << '\r' << std::flush;
	}
	else {
		compMan->GetGameState()->EndGame();
	}

	ImGui::SliderFloat("Sun rotation", (float*)&sunRotation, -10.0f, 10.0f);
	ImGui::SliderFloat3("Light Posiiton", (float*)&lightPos, 0.0f, 100.0f);
	ImGui::SliderFloat("Scale", (float*)&scale, 1.0f, 100.0f);
	ImGui::SliderFloat("Ortho", (float*)&o, 0.0f, 50.0f);

	// Sun direction
	//ImGui::SliderFloat("SUN ROTATIon", (float*)&sunRotation, -10.0, 10.0f);

	mat4 rotY = glm::rotate(mat4(1.0f), sunRotation, glm::vec3(1.0f, 0.0f, 0.0f));
	mat4 inverted = glm::inverse(rotY);
	sunDir = normalize(glm::vec3(inverted[2]));
	moonDir = vec3(sunDir.x, sunDir.y, sunDir.z * -1.0f);
}

mat4 LightComponent::GetOrthoMatrix()
{
	mat4 ortho = glm::ortho(-o, o, -o, o, -o, 200.0f);
	return ortho;
}

mat4 LightComponent::GetLightView()
{
	vec3 playerPos = componentManager->GetPlayer().GetPosition();
	vec3 camPos = playerPos + lightPos + (sunDir * scale);
	vec3 lookAt = camPos - sunDir;
	return glm::lookAt(camPos, lookAt, vec3(0, 1, 0));
	return mat4(1.0f);
}


void LightComponent::Init(ComponentManager* compMan)
{
	componentManager = compMan;
	ShaderManager shaderMan = ShaderManager::GetInstance();
	depthProg = shaderMan.GetShader("ShadowDepth");
	InitializeShadowFBO();
	initQuad();
}

void LightComponent::InitializeShadowFBO()
{
	//generate the FBO for the shadow depth
	glGenFramebuffers(1, &depthMapFBO);

	//generate the texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, S_WIDTH, S_HEIGHT,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//bind with framebuffer's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightComponent::SetupRenderShadows()
{
	//set up light's depth map
	glViewport(0, 0, S_WIDTH, S_HEIGHT);

	//sets up the output to be out FBO
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
}

void LightComponent::initQuad() {

	//now set up a simple quad for rendering FBO
	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);

	static const GLfloat g_quad_vertex_buffer_data[] = {
	 -1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 -1.0f,  1.0f, 0.0f,
	 -1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

void LightComponent::DebugDrawDepthTexture()
{
	glViewport(0, 0, 640, 640);

	// glBindFramebuffer(GL_FRAMEBUFFER, 0);
	initQuad();
	ShaderManager shaderMan = ShaderManager::GetInstance();
	shared_ptr<Program> debugProg = shaderMan.GetShader("TextureDebug");

	debugProg->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUniform1i(debugProg->getUniform("texBuf"), 0);

	//draw the quad
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	debugProg->unbind();
}