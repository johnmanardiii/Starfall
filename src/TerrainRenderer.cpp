#include "TerrainRenderer.h"
#include <iostream>

void TerrainRenderer::Init(ComponentManager* compMan)
{
	Renderer::Init(compMan);
}

void TerrainRenderer::Draw(float frameTime)
{
	prog->bind();
	Camera cam = Camera::GetInstance(vec3(0, 0, 0));
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &trans->GetModelMat()[0][0]);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &cam.GetPerspective()[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &cam.GetView()[0][0]);

	vec3 pos = -cam.GetPos();
	//std::cout << "Campos x: " << pos.x << " y: " << pos.y << " z: " << pos.z << std::endl;
	vec3 offset;
	vec3 color_offset;
	offset.y = 0;
	offset.x = (int)pos.x;
	offset.z = (int)pos.z;

	vec3 lightDir = normalize(vec3(0, -1, 1)); // Hardcoded for now

	glUniform3fv(prog->getUniform("camoff"), 1, &offset[0]);
	glUniform3fv(prog->getUniform("campos"), 1, &pos[0]);
	glUniform3fv(prog->getUniform("lightDir"), 1, &lightDir[0]);
	glBindVertexArray(terrain.VAOId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain.IndexBuff);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glDrawElements(GL_TRIANGLES, terrain.numVerts, GL_UNSIGNED_SHORT, (void*)0);
	prog->unbind();
}
