#include "DroneRenderer.h"

void DroneRenderer::Init(ComponentManager* compMan)
{
	Renderer::Init(compMan);
	GameObject gameObject = compMan->GetGameObject(Name);
	int index = gameObject.GetComponentLocation("DroneManager");
	drone_manager = static_pointer_cast<DroneManager>(compMan->GetComponent("DroneManager", index));
}

void DroneRenderer::Draw(float frameTime)
{
	prog->bind();
	// send over PVM matrices
	Camera cam = Camera::GetInstance(vec3(0, 1, 0));
	mat4 P = cam.GetPerspective(),
		V = cam.GetView(),
		M;
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, base_tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, emissive_tex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normal_tex);

	vector<mat4> MMatrices = drone_manager->GetModelMatrices();
	for (int i = 0; i < MMatrices.size(); i++)
	{
		M = MMatrices[i];
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		model->draw(prog, true);
	}
	prog->unbind();
}