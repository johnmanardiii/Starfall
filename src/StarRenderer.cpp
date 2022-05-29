#include "StarRenderer.h"
#include "GameObject.h"
#include "ComponentManager.h"
void StarRenderer::Update(float frameTime, ComponentManager* compMan) {
	totalTime += frameTime;
	
	Draw(frameTime);
}

void StarRenderer::Init(ComponentManager* compMan)
{
	
    Renderer::Init(compMan);
	state = compMan->GetGameState();
	isCullable = true; //set the star fragments as being available to be culled.
	//GameObject& obj = compMan->GetGameObject(Name);
	shared_ptr<Collision> col = static_pointer_cast<Collision>(compMan->GetComponent("Collision", compMan->GetGameObject(Name).GetComponentLocation("Collision")));
	col->Init(compMan);
	//set their culling radius to be the same as the radius of the collision component.
	//Other derived renderer components may choose to set a custom culling radius e.g. if they are not collidable.
	cullingRadius = col->getRadius();
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
	vec3 campos = cam.GetPos();
	glUniform3f(prog->getUniform("campos"), campos.x, campos.y, campos.z);
	
	glUniform3fv(prog->getUniform("lights"), state->lights.size(), state->lights.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	model->draw(prog);

    prog->unbind();
}
