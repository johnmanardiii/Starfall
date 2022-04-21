#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include "particleSys.h"
#include "GLSL.h"
#include "RandomGenerator.h"

using namespace std;

float particleSys::randFloat(float l, float h)
{
	float r = rand() / (float) RAND_MAX;
	return (1.0f - r) * l + r * h;
}

particleSys::particleSys(vec3 source) {

	numP = 300;	
	t = 0.0f;
	h = 0.01f;
	g = vec3(0.0f, -0.098, 0.0f);
	start = source;
	theCamera = glm::mat4(1.0);
}

void particleSys::gpuSetup() {

  cout << "start: " << start.x << " " << start.y << " " <<start.z << endl;
	for (int i=0; i < numP; i++) {
		points[i*3+0] = start.x;
		points[i*3+1] = start.y;
		points[i*3+2] = start.z;
		RandomGenerator rg(0.0, 0.4);
		vec4 color = (vec4(rg.GetVec3(), 0.0) + vec4(0.4, 0.2, 0.2, 1.0));
		auto particle = make_shared<Particle>(start, color);
		particles.push_back(particle);
		particle->load(start);

		//To do - how can you integrate unique colors per particle?
		pointColors[i * 3 + 0] = particles.at(i)->getColor().r;
		pointColors[i * 3 + 1] = particles.at(i)->getColor().g;
		pointColors[i * 3 + 2] = particles.at(i)->getColor().b;
		
	}

	//generate the VAO
   glGenVertexArrays(1, &vertArrObj);
   glBindVertexArray(vertArrObj);

   //generate vertex buffer to hand off to OGL - using instancing
   glGenBuffers(1, &vertBuffObj);
   //set the current state to focus on our vertex buffer
   glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
   //actually memcopy the data - only do this once
   glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points[0], GL_STREAM_DRAW);
   
   glGenBuffers(1, &colBuffObj);
   glBindBuffer(GL_ARRAY_BUFFER, colBuffObj);
   glBufferData(GL_ARRAY_BUFFER, sizeof(pointColors), &pointColors[0], GL_STREAM_DRAW);

   assert(glGetError() == GL_NO_ERROR);
}

void particleSys::reSet(vec3) {
	for (int i=0; i < numP; i++) {
		particles[i]->load(start);
	}
}

void particleSys::drawMe(std::shared_ptr<Program> prog) {
	prog->bind();
	glBindVertexArray(vertArrObj);

	int h_pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int c_pos = prog->getAttribute("pColor");
	GLSL::enableVertexAttribArray(c_pos);
	glBindBuffer(GL_ARRAY_BUFFER, colBuffObj);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(pointColors), NULL, GL_STREAM_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numP * 3, pointColors);



	
	//glVertexAttribDivisor(0, 1);
	//glVertexAttribDivisor(1, 1);

	// Draw the points !
	glDrawArraysInstanced(GL_POINTS, 0, 1, numP);

	//glVertexAttribDivisor(0, 0);
	//glVertexAttribDivisor(1, 0);
	//std::cout << "Any Gl errors2: " << glGetError() << std::endl;
	glDisableVertexAttribArray(0);
	prog->unbind();
}

void particleSys::update() {

  vec3 pos;
  vec4 col;

  //update the particles
  for(auto particle : particles) {
        particle->update(t, h, g, start);
  }
  t += h;
 
  // Sort the particles by Z
  //temp->rotate(camRot, vec3(0, 1, 0));
  //be sure that camera matrix is updated prior to this update
  vec3 s, t, sk;
  vec4 p;
  quat r;
  glm::decompose(theCamera, s, r, t, sk, p);
  sorter.C = glm::toMat4(r); 
  sort(particles.begin(), particles.end(), sorter);


  //go through all the particles and update the CPU buffer

  //update the GPU data
   //verts
   glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
   glBufferData(GL_ARRAY_BUFFER, sizeof(points), NULL, GL_STREAM_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*numP*3, points);
   //colors
   glBindBuffer(GL_ARRAY_BUFFER, colBuffObj);
   glBufferData(GL_ARRAY_BUFFER, sizeof(pointColors), NULL, GL_STREAM_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*numP*3, pointColors);
   
   //unbind
   glBindBuffer(GL_ARRAY_BUFFER, 0);

}
