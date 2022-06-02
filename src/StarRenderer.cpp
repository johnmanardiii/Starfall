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
	glUniform3fv(prog->getUniform("playerPos"), 1, glm::value_ptr(Player::GetInstance(vec3()).GetPosition()));
	vec3 campos = cam.GetPos();
	glUniform3f(prog->getUniform("campos"), campos.x, campos.y, campos.z);
	
	glUniform3fv(prog->getUniform("lights"), state->lights.size(), state->lights.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	model->draw(prog, false);

    prog->unbind();
}

void StarRenderer::DrawDepth()
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
	model->draw(dprog);
	dprog->unbind();
}
