#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include "particleSys.h"
#include "GLSL.h"
#include "RandomGenerator.h"
#include "ShaderManager.h"

using namespace std;

float particleSys::randFloat(float l, float h)
{
	float r = rand() / (float) RAND_MAX;
	return (1.0f - r) * l + r * h;
}

particleSys::particleSys(int numParticles, vec3 source) {
	points = vector<GLfloat>(numParticles * 3);
	pointColors = vector<float>(numParticles * 3);
	numP = numParticles;	
	totalTime = 0.0f;
	g = vec3(0.0f, -0.098, 0.0f);
	
	start = source;
	cout << "here" << start.x << " " << start.y << " " << start.z << endl;
	View = glm::mat4(1.0);
}

void particleSys::gpuSetup() {
	for (int i=0; i < numP; i++) {
		points[i*3+0] = start.x;
		points[i*3+1] = start.y;
		points[i*3+2] = start.z;
		vec4 color = (vec4(randFloat(0, 0.4), randFloat(0, 0.4), randFloat(0, 0.4), 0.0) + vec4(0.4, 0.2, 0.2, 1.0));
		auto particle = make_shared<Particle>(start, color);
		particles.push_back(particle);
		particle->rebirth(totalTime, start);

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
   glBufferData(GL_ARRAY_BUFFER, points.size(), reinterpret_cast<GLfloat*>(points.data()), GL_STREAM_DRAW);
   
   glGenBuffers(1, &colBuffObj);
   glBindBuffer(GL_ARRAY_BUFFER, colBuffObj);
   glBufferData(GL_ARRAY_BUFFER, pointColors.size(), reinterpret_cast<GLfloat*>(pointColors.data()), GL_STREAM_DRAW);

   assert(glGetError() == GL_NO_ERROR);
}


void particleSys::drawMe(std::shared_ptr<Program> prog, shared_ptr<Transform> trans) {
	prog->bind();
	glBindVertexArray(vertArrObj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ShaderManager::GetInstance().GetTexture("Alpha"));
	cout << start.x << " " << start.y << " " << start.z << endl;
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, glm::value_ptr(View));
	mat4 Model = glm::translate(mat4(1.0f),trans->GetPos());
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model));

	int v_pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(v_pos);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
	glVertexAttribPointer(v_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, points.size(), reinterpret_cast<GLfloat*>(points.data()), GL_STREAM_DRAW);
	glVertexAttribDivisor(v_pos, 1);

	int c_pos = prog->getAttribute("pColor");
	GLSL::enableVertexAttribArray(c_pos);
	glBindBuffer(GL_ARRAY_BUFFER, colBuffObj);
	glVertexAttribPointer(c_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, pointColors.size(), reinterpret_cast<GLfloat*>(pointColors.data()), GL_STREAM_DRAW);
	glVertexAttribDivisor(c_pos, 1);
	
	// Draw the points
	glDrawArraysInstanced(GL_POINTS, 0, 1, numP);
	//reset, no instancing.
	
	//std::cout << "Any Gl errors2: " << glGetError() << std::endl;
	//glDisableVertexAttribArray(0);
	prog->unbind();
}

void particleSys::update(float frameTime, shared_ptr<Transform> trans) {
	
    //update the particles
	for (int i = 0; i < particles.size(); i++) {
        particles[i]->update(totalTime, frameTime, g, trans->GetPos());
		points[i * 3 + 0] = particles.at(i)->getPosition().x;
		points[i * 3 + 1] = particles.at(i)->getPosition().y;
		points[i * 3 + 2] = particles.at(i)->getPosition().z;
    }
  
    totalTime += frameTime;
 
    // Sort the particles by Z
    //be sure that camera matrix is updated prior to this update
    vec3 s, t, sk;
    vec4 p;
    quat r;
    glm::decompose(View, s, r, t, sk, p);
    sorter.C = glm::toMat4(r); 
    sort(particles.begin(), particles.end(), sorter);
}
