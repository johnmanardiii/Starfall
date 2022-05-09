#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include "particleSys.h"
#include "GLSL.h"
#include "GLFW/glfw3.h"
#include "RandomGenerator.h"
#include "ShaderManager.h"

using namespace std;

int particleSys::currBufObjs = 0;
vector<vector<float>> particleSys::pointColors;
vector<vector<float>> particleSys::pointNormals;
vector<vector<float>> particleSys::pointRotations;

vector<unsigned> particleSys::vertArrObj = vector<unsigned>(numUniqueBufObjs);
vector<unsigned> particleSys::colBufObj  = vector<unsigned>(numUniqueBufObjs);
vector<unsigned> particleSys::norBufObj  = vector<unsigned>(numUniqueBufObjs);
vector<unsigned> particleSys::rotBufObj  = vector<unsigned>(numUniqueBufObjs);


float randFloat(float l, float h)
{
	float r = rand() / (float)RAND_MAX;
	return (1.0f - r) * l + r * h;
}

particleSys::particleSys(int numParticles, vec3 source) {
	numP = numParticles;	
	
	start = source;
	View = glm::mat4(1.0);
	bufObjIndex = particleSys::currBufObjs % numUniqueBufObjs;
	particleSys::currBufObjs++;
}

void particleSys::gpuSetup(std::shared_ptr<Program> prog, int numP) {
	for (int i = 0; i < particleSys::numUniqueBufObjs; i++) {
		vertArrObj.push_back(0);
		pointColors.emplace_back(vector<float>(numP * 3));
		pointNormals.emplace_back(vector<float>(numP * 3));
		pointRotations.emplace_back(vector<float>(numP * 3));


		for (int j = 0; j < numP; j++) {
			vec4 color = (vec4(randFloat(0, 0.4), randFloat(0, 0.4), randFloat(0, 0.4), 0.0) + vec4(0.4, 0.2, 0.2, 1.0));
			pointColors[i][j * 3 + 0] = color.r;
			pointColors[i][j * 3 + 1] = color.g;
			pointColors[i][j * 3 + 2] = color.b;

			vec3 normal = normalize(vec3(randFloat(-1, 1), randFloat(-1, 1), randFloat(-1, 1)));
			pointNormals[i][j * 3 + 0] = normal.x;
			pointNormals[i][j * 3 + 1] = normal.y;
			pointNormals[i][j * 3 + 2] = normal.z;

			pointRotations[i][j * 3 + 0] = randFloat(-1, 1);
			pointRotations[i][j * 3 + 1] = randFloat(-1, 1);
			pointRotations[i][j * 3 + 2] = randFloat(-1, 1);
		}

		//generate the VAO
		glGenVertexArrays(1, &vertArrObj[i]);
		glBindVertexArray(vertArrObj[i]);

		glGenBuffers(1, &colBufObj[i]);
		int c_pos = prog->getAttribute("pColor");
		GLSL::enableVertexAttribArray(c_pos);
		glBindBuffer(GL_ARRAY_BUFFER, colBufObj[i]);
		glVertexAttribPointer(c_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBufferData(GL_ARRAY_BUFFER, pointColors[i].size(), reinterpret_cast<GLfloat*>(pointColors[i].data()), GL_STREAM_DRAW);
		glVertexAttribDivisor(c_pos, 1);

		glGenBuffers(1, &norBufObj[i]);
		int n_pos = prog->getAttribute("pNormal");
		GLSL::enableVertexAttribArray(n_pos);
		glBindBuffer(GL_ARRAY_BUFFER, norBufObj[i]);
		glVertexAttribPointer(n_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBufferData(GL_ARRAY_BUFFER, pointNormals[i].size(), reinterpret_cast<GLfloat*>(pointNormals[i].data()), GL_STREAM_DRAW);
		glVertexAttribDivisor(n_pos, 1);

		glGenBuffers(1, &rotBufObj[i]);
		int r_pos = prog->getAttribute("pRotation");
		GLSL::enableVertexAttribArray(r_pos);
		glBindBuffer(GL_ARRAY_BUFFER, rotBufObj[i]);
		glVertexAttribPointer(r_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBufferData(GL_ARRAY_BUFFER, pointRotations[i].size(), reinterpret_cast<GLfloat*>(pointRotations[i].data()), GL_STREAM_DRAW);
		glVertexAttribDivisor(r_pos, 1);

		//assert(glGetError() == GL_NO_ERROR);
	}
}


void particleSys::drawMe(std::shared_ptr<Program> prog, shared_ptr<Transform> trans, float totalTime) {
	prog->bind();
	glBindVertexArray(vertArrObj[bufObjIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, colBufObj[bufObjIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObj[bufObjIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, rotBufObj[bufObjIndex]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ShaderManager::GetInstance().GetTexture("Alpha"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ShaderManager::GetInstance().GetTexture("Rainbow"));

	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, glm::value_ptr(View));
	mat4 Model = glm::translate(mat4(1.0f),trans->GetPos());
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model));
	glUniform1f(prog->getUniform("totalTime"), totalTime);
	glUniform3f(prog->getUniform("centerPos"), trans->GetPos().x, trans->GetPos().y, trans->GetPos().z);
	glUniform1f(prog->getUniform("alphaMult"), clamp(1-totalTime, 0.0f, 1.0f));
	// Draw the points
	glDrawArraysInstanced(GL_POINTS, 0, 1, numP);
	
	prog->unbind();
}

/*void particleSys::update(float frameTime, shared_ptr<Transform> trans) {
    // Sort the particles by Z
    //be sure that camera matrix is updated prior to this update
    vec3 s, t, sk;
    vec4 p;
    quat r;
    glm::decompose(View, s, r, t, sk, p);
    sorter.C = glm::toMat4(r); 
    sort(particles.begin(), particles.end(), sorter);
}*/
