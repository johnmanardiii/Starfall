#include "SkyboxRenderer.h"
#include "Camera.h"
#include "LightComponent.h"
#include <iostream>

void SkyboxRenderer::Init(ComponentManager* compMan)
{
	Renderer::Init(compMan);
}

void SkyboxRenderer::UpdateUniforms()
{
	ImGui::ColorEdit3("Day Top Color", (float*)&dayTopColor);
	glUniform3fv(prog->getUniform("dayTopColor"), 1, &dayTopColor[0]);

	ImGui::ColorEdit3("Day Bottom Color", (float*)&dayBottomColor);
	glUniform3fv(prog->getUniform("dayBottomColor"), 1, &dayBottomColor[0]);

	ImGui::ColorEdit3("Night Top Color", (float*)&nightTopColor);
	glUniform3fv(prog->getUniform("nightTopColor"), 1, &nightTopColor[0]);

	ImGui::ColorEdit3("Night Bottom Color", (float*)&nightBottomColor);
	glUniform3fv(prog->getUniform("nightBottomColor"), 1, &nightBottomColor[0]);

	ImGui::ColorEdit3("Horizon Color", (float*)&horizonColor);
	glUniform3fv(prog->getUniform("horizonColor"), 1, &horizonColor[0]);

	LightComponent light = LightComponent::GetInstance(vec3(1, 0, 0));

	ImGui::SliderFloat3("Light Direction",(float*)&sunDir, -1.0f, 1.0f);
	glUniform3fv(prog->getUniform("sunDir"), 1, &sunDir[0]);

	ImGui::SliderFloat("MoonOffset", (float*)&moonOffset, -1.0f, 1.0f);
	glUniform1f(prog->getUniform("moonOffset"), moonOffset);
}

void SkyboxRenderer::Draw(float frameTime)
{
	prog->bind();

	UpdateUniforms();

	Camera cam = Camera::GetInstance(vec3(0, 1, 0));
	vec3 pos = cam.GetPos();
	trans->SetPos(vec3(pos.x, 0, pos.z));
	ShaderManager shaderMan = ShaderManager::GetInstance();
	mat4 P = cam.GetPerspective(),
		V = cam.GetView(),
		M = trans->GetModelMat();
	glDepthFunc(GL_LEQUAL);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	glUniform1f(prog->getUniform("time"), glfwGetTime());

	glBindTexture(GL_TEXTURE_CUBE_MAP, shaderMan.skyboxTexId);
	model->draw(prog);
	glDepthFunc(GL_LESS);
	prog->unbind();
}
