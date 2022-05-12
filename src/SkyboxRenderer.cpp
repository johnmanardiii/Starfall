#include "SkyboxRenderer.h"
#include "Camera.h"
#include "LightComponent.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

void SkyboxRenderer::Init(ComponentManager* compMan)
{
	Renderer::Init(compMan);
}

void SkyboxRenderer::UpdateUniforms()
{
	// Gradient
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

	ImGui::SliderFloat("Sun Rotation", (float*)&sunRotation, 0.0f, 10.0f);
	glUniform3fv(prog->getUniform("sunDir"), 1, &sunDir[0]);

	ImGui::SliderFloat("MoonOffset", (float*)&moonOffset, -1.0f, 1.0f);
	glUniform1f(prog->getUniform("moonOffset"), moonOffset);

	// Clouds
	ImGui::SliderFloat("Cloud Fuzziness", (float*)&cloudFuzziness, 0.0f, 1.0f);
	glUniform1f(prog->getUniform("cloudFuzziness"), cloudFuzziness);

	ImGui::SliderFloat("cloudScale", (float*)&cloudScale, 0.0f, 1.0f);
	glUniform1f(prog->getUniform("cloudScale"), cloudScale);

	ImGui::SliderFloat("cloudSpeed", (float*)&cloudSpeed, 0.0f, 1.0f);
	glUniform1f(prog->getUniform("cloudSpeed"), cloudSpeed);

	ImGui::SliderFloat("cloudCutoff", (float*)&cloudCutoff, 0.0f, 1.0f);
	glUniform1f(prog->getUniform("cloudCutoff"), cloudCutoff);

	ImGui::ColorEdit3("cloudColorDayEdge", (float*)&cloudColorDayEdge);
	glUniform3fv(prog->getUniform("cloudColorDayEdge"), 1, &cloudColorDayEdge[0]);

	ImGui::ColorEdit3("cloudColorDayMain", (float*)&cloudColorDayMain);
	glUniform3fv(prog->getUniform("cloudColorDayMain"), 1, &cloudColorDayMain[0]);

	ImGui::ColorEdit3("cloudColorNightEdge", (float*)&cloudColorNightEdge);
	glUniform3fv(prog->getUniform("cloudColorNightEdge"), 1, &cloudColorNightEdge[0]);

	ImGui::ColorEdit3("cloudColorNightMain", (float*)&cloudColorNightMain);
	glUniform3fv(prog->getUniform("cloudColorNightMain"), 1, &cloudColorNightMain[0]);

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
	
	// Sun direction
	mat4 rotY = glm::rotate(mat4(1.0f), sunRotation, glm::vec3(1.0f, 0.0f, 0.0f));
	mat4 inverted = glm::inverse(rotY);
	sunDir = normalize(glm::vec3(inverted[2]));
	glDepthFunc(GL_LEQUAL);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	glUniform1f(prog->getUniform("time"), glfwGetTime());

	// Textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, cloudBaseNoise);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, cloudNoise);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, cloudDistort);

	glBindTexture(GL_TEXTURE_CUBE_MAP, shaderMan.skyboxTexId);
	model->draw(prog);
	glDepthFunc(GL_LESS);
	prog->unbind();
}
