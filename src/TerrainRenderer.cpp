 #include "TerrainRenderer.h"
#include "Player.h"
#include <iostream>

void TerrainRenderer::Init(ComponentManager* compMan)
{
	Renderer::Init(compMan);
}

void TerrainRenderer::UpdateUniforms()
{

}

void TerrainRenderer::Draw(float frameTime)
{
	prog->bind();
	Camera cam = Camera::GetInstance(vec3(0, 0, 0));
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &trans->GetModelMat()[0][0]);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &cam.GetPerspective()[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &cam.GetView()[0][0]);

	vec3 pos = cam.GetPos();
	vec3 offset = Player::GetInstance(vec3(0,0,0)).GetPosition();
	vec3 color_offset;
	offset.y = 0;
	offset.x = (int)offset.x;
	offset.z = (int)offset.z;
	vec3 lightDir = normalize(vec3(1000, 0, 100)); // Hardcoded for now

	glUniform3fv(prog->getUniform("camoff"), 1, &offset[0]);
	glUniform3fv(prog->getUniform("campos"), 1, &pos[0]);
	glUniform3fv(prog->getUniform("lightDir"), 1, &lightDir[0]);
	glUniform1f(prog->getUniform("time"), glfwGetTime());
	glBindVertexArray(terrain.VAOId);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain.IndexBuff);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shallowTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, steepTexture);

	glDrawElements(GL_TRIANGLES, terrain.numVerts, GL_UNSIGNED_SHORT, (void*)0);
	prog->unbind();
	glDisable(GL_BLEND);
}
