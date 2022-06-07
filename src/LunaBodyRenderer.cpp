#include "LunaBodyRenderer.h"
#include "Camera.h"
#include "ComponentManager.h"
#include "LightComponent.h"
#include <iostream>

void LunaBodyRenderer::Init(ComponentManager* compMan)
{
	Renderer::Init(compMan);
}

void LunaBodyRenderer::Draw(float frameTime)
{
	prog->bind();
	// send over PVM matrices
	Camera cam = Camera::GetInstance(vec3(0,1,0));
	LightComponent light = LightComponent::GetInstance(vec3(0));
	mat4 P = cam.GetPerspective(),
		V = cam.GetView(),
		M = trans->GetModelMat();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	glUniform3fv(prog->getUniform("flashCol"), 1, &flashColor[0]);
	glUniform3fv(prog->getUniform("camPos"), 1, &cm->GetCamera().GetPos()[0]);
	glUniform1f(prog->getUniform("flashAmt"), flashAmt);
	glUniform3fv(prog->getUniform("lightDir"), 1, &light.moonDir[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, emissive_tex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normal_tex);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, roughness);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, metal);
	model->draw(prog, true);
	prog->unbind();
}

void LunaBodyRenderer::DrawDepth()
{
	// send over PVM matrices
	Camera cam = Camera::GetInstance(vec3(0, 1, 0));
	LightComponent lightC = LightComponent::GetInstance(vec3(0));
	shared_ptr<Program> dprog = lightC.depthProg;
	mat4 ortho = lightC.GetOrthoMatrix();
	mat4 LV = lightC.GetLightView();
	mat4 M = trans->GetModelMat();
	

	dprog->bind();
	glUniformMatrix4fv(dprog->getUniform("LP"), 1, GL_FALSE, &ortho[0][0]);
	glUniformMatrix4fv(dprog->getUniform("LV"), 1, GL_FALSE, &LV[0][0]);
	glUniformMatrix4fv(dprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	glUniform1i(dprog->getUniform("castShadows"), 1);
	model->draw(dprog, false);
	dprog->unbind();
}