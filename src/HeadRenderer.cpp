#include "HeadRenderer.h"
#include "LightComponent.h"

#include "ComponentManager.h"
#include <iostream>

void HeadRenderer::Init(ComponentManager* compMan)
{
	Renderer::Init(compMan);
}

float CosInterpolate(float v1, float v2, float a)
{
	float angle = a * glm::pi<float>();
	float prc = (1.0f - cos(angle)) * 0.5f;
	return  v1 * (1.0f - prc) + v2 * prc;
}


void HeadRenderer::Draw(float frameTime)
{
	totalTime += frameTime;
	eyeOpenPct = (glm::cos(glm::min<float>(fmod(totalTime, 5), 1.0f) * 2. * glm::pi<float>()) + 1) / 2.0;
	prog->bind();
	// send over PVM matrices
	Camera cam = Camera::GetInstance(vec3(0,1,0));
	mat4 P = cam.GetPerspective(),
		V = cam.GetView(),
		M = trans->GetModelMat();
	LightComponent light = LightComponent::GetInstance(vec3(1, 0, 0));
	glUniform3fv(prog->getUniform("lightDir"), 1, &light.moonDir[0]);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	glUniform3fv(prog->getUniform("flashCol"), 1, &flashColor[0]);
	glUniform1f(prog->getUniform("flashAmt"), flashAmt);
	glUniform2fv(prog->getUniform("eye1Pos"), 1, &eye1Pos[0]);
	glUniform1f(prog->getUniform("eye1Radius"), eye1Radius);
	glUniform1f(prog->getUniform("eyeOpenPct"), eyeOpenPct);
	glUniform3fv(prog->getUniform("camPos"), 1, &cm->GetCamera().GetPos()[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	model->draw(prog, false);
	prog->unbind();
}

void HeadRenderer::DrawDepth()
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