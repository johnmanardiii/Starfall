#include "TerrainRenderer.h"
#include "LightComponent.h"

#include "imgui/imgui.h"
#include "Player.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

const float TerrainRenderer::OFFSET_FROM_CENTER = 20.0f; 
const float TerrainRenderer::TERRAIN_SIZE = 300.0f;
const float TerrainRenderer::BASE_HEIGHT = -4.0f;

void TerrainRenderer::Init(ComponentManager* compMan)
{
	Renderer::Init(compMan);
}

void TerrainRenderer::UpdateUniforms()
{
	if (ImGui::CollapsingHeader("Terrain!")) {}

	LightComponent light = LightComponent::GetInstance(vec3(1, 0, 0));

	glUniform3fv(prog->getUniform("lightDir"), 1, &light.moonDir[0]);

	// Diffuse
	ImGui::SliderFloat("diffuseContrast", (float*)&diffuseContrast, -1.0f, 10.0f);
	glUniform1f(prog->getUniform("diffuseContrast"), diffuseContrast);

	ImGui::ColorEdit3("shadowColor", (float*)&shadowColor);
	glUniform3fv(prog->getUniform("shadowColor"), 1, &shadowColor[0]);

	ImGui::ColorEdit3("terrainColor", (float*)&terrainColor);
	glUniform3fv(prog->getUniform("terrainColor"), 1, &terrainColor[0]);

	ImGui::SliderFloat("sandStrength", (float*)&sandStrength, 0.0f, 1.0f);
	glUniform1f(prog->getUniform("sandStrength"), sandStrength);

	// Rim
	ImGui::SliderFloat("rimStrength", (float*)&rimStrength, 0.0f, 5.0f);
	glUniform1f(prog->getUniform("rimStrength"), rimStrength);

	ImGui::SliderFloat("rimPower", (float*)&rimPower, -10.0f, 100.0f);
	glUniform1f(prog->getUniform("rimPower"), rimPower);

	ImGui::ColorEdit3("rimColor", (float*)&rimColor);
	glUniform3fv(prog->getUniform("rimColor"), 1, &rimColor[0]);

	// Ocean Spec
	ImGui::SliderFloat("oceanSpecularStrength", (float*)&oceanSpecularStrength, 0.0f, 10.0f);
	glUniform1f(prog->getUniform("oceanSpecularStrength"), oceanSpecularStrength);

	ImGui::SliderFloat("oceanSpecularPower", (float*)&oceanSpecularPower, 0.0f, 200.0f);
	glUniform1f(prog->getUniform("oceanSpecularPower"), oceanSpecularPower);

	ImGui::ColorEdit3("oceanSpecularColor", (float*)&oceanSpecularColor);
	glUniform3fv(prog->getUniform("oceanSpecularColor"), 1, &oceanSpecularColor[0]);

	// Sand Ripples
	ImGui::SliderFloat("steepnessSharpnessPower", (float*)&steepnessSharpnessPower, -10.0f, 10.0f);
	glUniform1f(prog->getUniform("steepnessSharpnessPower"), steepnessSharpnessPower);

	ImGui::SliderFloat("specularHardness", (float*)&specularHardness, -10.0, 1.0f);
	glUniform1f(prog->getUniform("specularHardness"), specularHardness);

	ImGui::SliderFloat("sandRipplesStrength", (float*)&sandRipplesStrength, 0.0, 10.0f);
	glUniform1f(prog->getUniform("sandRipplesStrength"), sandRipplesStrength);
}

void TerrainRenderer::UpdatePosition()
{
	Player player = Player::GetInstance(vec3(0, 0, 0));
	quat rot = player.GetRotation();
	trans->SetRot(rot);
	vec3 offset = rot * vec3(-TERRAIN_SIZE / 2.0f, 0, -TERRAIN_SIZE + OFFSET_FROM_CENTER);
	vec3 playerPos = player.GetPosition();
	playerPos.y = BASE_HEIGHT;
	trans->SetPos(playerPos + offset);
}

void TerrainRenderer::Draw(float frameTime)
{
	prog->bind();

	UpdateUniforms();
	UpdatePosition();
	
	Camera cam = Camera::GetInstance(vec3(0, 0, 0));
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &trans->GetModelMat()[0][0]);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &cam.GetPerspective()[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &cam.GetView()[0][0]);

	LightComponent lightC = LightComponent::GetInstance(vec3(0));
	shared_ptr<Program> dprog = lightC.depthProg;
	mat4 ortho = lightC.GetOrthoMatrix();
	mat4 LV = lightC.GetLightView();
	mat4 LS = ortho * LV;

	vec3 pos = cam.GetPos();

	glUniformMatrix4fv(prog->getUniform("LS"), 1, GL_FALSE, &LS[0][0]);

	//vec3 lightDir = normalize(vec3(1000, 0, 100)); // Hardcoded for now

	// Sun direction
	mat4 rotY = glm::rotate(mat4(1.0f), sunRotation, vec3(1.0f, 0.0f, 0.0f));
	mat4 inverted = glm::inverse(rotY);
	sunDir = normalize(vec3(inverted[2]));

	vec3 playerPos = Player::GetInstance(vec3()).GetPosition();
	glUniform3fv(prog->getUniform("campos"), 1, &pos[0]);
	glUniform3fv(prog->getUniform("playerPos"), 1, &playerPos[0]);
	glUniform1f(prog->getUniform("baseHeight"), BASE_HEIGHT);
	
	glUniform1f(prog->getUniform("time"), glfwGetTime());
	glBindVertexArray(terrain.VAOId);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain.IndexBuff);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shallowTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, steepTexture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, lightC.depthMap);

	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawElements(GL_PATCHES, terrain.numVerts, GL_UNSIGNED_SHORT, (void*)0);
	prog->unbind();
	glDisable(GL_BLEND);
}

void TerrainRenderer::DrawDepth()
{
	// send over PVM matrices
	Camera cam = Camera::GetInstance(vec3(0));
	LightComponent lightC = LightComponent::GetInstance(vec3(0));
	mat4 ortho = lightC.GetOrthoMatrix();
	mat4 LV = lightC.GetLightView();
	mat4 M = trans->GetModelMat();

	depthProg->bind();

	glUniformMatrix4fv(depthProg->getUniform("M"), 1, GL_FALSE, &trans->GetModelMat()[0][0]);

	vec3 pos = cam.GetPos();
	vec3 offset;
	vec3 color_offset;
	offset.y = 0;
	offset.x = (int)pos.x;
	offset.z = (int)pos.z;

	// Sun direction
	mat4 rotY = glm::rotate(mat4(1.0f), sunRotation, glm::vec3(1.0f, 0.0f, 0.0f));
	mat4 inverted = glm::inverse(rotY);
	sunDir = normalize(glm::vec3(inverted[2]));

	glUniformMatrix4fv(depthProg->getUniform("LP"), 1, GL_FALSE, &ortho[0][0]);
	glUniformMatrix4fv(depthProg->getUniform("LV"), 1, GL_FALSE, &LV[0][0]);
	glUniform1i(depthProg->getUniform("castShadows"), 0);

	glBindVertexArray(terrain.VAOId);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain.IndexBuff);

	glDrawElements(GL_TRIANGLES, terrain.numVerts, GL_UNSIGNED_SHORT, (void*)0);
	depthProg->unbind();
}
