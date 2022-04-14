#include "TextureRenderer.h"
#include <iostream>

void TextureRenderer::Init(ComponentManager& compMan)
{
	Renderer::Init(compMan);
}

void TextureRenderer::Draw(float frameTime)
{
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	model->draw(prog);
	prog->unbind();
}