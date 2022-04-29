#include "StarRenderer.h"

void StarRenderer::Init(ComponentManager* compMan)
{
    Renderer::Init(compMan);
}

void StarRenderer::Draw(float frameTime)
{
    prog->bind();

	Camera cam = Camera::GetInstance(vec3(0, 1, 0));
	mat4 P = cam.GetPerspective(),
		V = cam.GetView(),
		M = trans->GetModelMat();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P));
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M));


    prog->unbind();
}
