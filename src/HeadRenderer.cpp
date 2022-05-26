#include "HeadRenderer.h"
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
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	glUniform3fv(prog->getUniform("flashCol"), 1, &flashColor[0]);
	glUniform1f(prog->getUniform("flashAmt"), flashAmt);
	glUniform2fv(prog->getUniform("eye1Pos"), 1, &eye1Pos[0]);
	glUniform1f(prog->getUniform("eye1Radius"), eye1Radius);
	glUniform1f(prog->getUniform("eyeOpenPct"), eyeOpenPct);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	model->draw(prog);
	prog->unbind();
}