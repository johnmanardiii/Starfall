#include "StarRenderer.h"

void StarRenderer::Update(float frameTime, ComponentManager* compMan) {
	totalTime += frameTime;
	Draw(frameTime);
}

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

	glUniform1f(prog->getUniform("totalTime"), totalTime);
	vec3 pos = trans->GetPos();
	glUniform3f(prog->getUniform("centerPos"), pos.x, pos.y, pos.z);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	model->draw(prog);

    prog->unbind();
}
