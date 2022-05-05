#include "SkyboxRenderer.h"
#include "Camera.h"
#include <iostream>

void SkyboxRenderer::Init(ComponentManager* compMan)
{
	Renderer::Init(compMan);
}

void SkyboxRenderer::Draw(float frameTime)
{
	prog->bind();

	Camera cam = Camera::GetInstance(vec3(0, 1, 0));
	mat4 P = cam.GetPerspective(),
		V = cam.GetView(),
		M = trans->GetModelMat();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	model->draw(prog);
	prog->unbind();
}
