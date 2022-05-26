#include "ParticleStaticSplash.h"
#include "ComponentManager.h"
using namespace std;

int ParticleRenderer::currBufObjs = 0;
vector<vector<float>> ParticleRenderer::pointColors;
vector<vector<float>> ParticleRenderer::pointNormals;
vector<vector<float>> ParticleRenderer::pointRotations;

vector<unsigned> ParticleRenderer::vertArrObj = vector<unsigned>(numUniqueBufObjs);
vector<unsigned> ParticleRenderer::colBufObj = vector<unsigned>(numUniqueBufObjs);
vector<unsigned> ParticleRenderer::norBufObj = vector<unsigned>(numUniqueBufObjs);
vector<unsigned> ParticleRenderer::rotBufObj = vector<unsigned>(numUniqueBufObjs);


vector<pair<int, int>>  ParticleRenderer::SpriteRowColumnTable;


float randFloat(float l, float h)
{
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * l + r * h;
}


void ParticleRenderer::Update(float frameTime, ComponentManager* compMan)
{   
	
    totalTime += frameTime;
	this->frametime = frameTime;
	if (totalTime > 4) {
		compMan->RemoveGameObject(Name);
		return;
	}
    //reset the particle orientation to the camera's view matrix.
    setCamera(compMan->GetCamera().GetView());
    setProjection(compMan->GetCamera().GetPerspective());
}

void ParticleRenderer::Draw(float frameTime)
{
	(this->*func)(totalTime);
}

void ParticleRenderer::Init(ComponentManager* compMan)
{
    //link transform component
    GameObject obj = compMan->GetGameObject(Name);
    int index = obj.GetComponentLocation("Transform");
    trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));
    start = trans->GetPos();

	//declare the particle renderer as cullable and set a reasonable culling radius
	isCullable = true;
}

void ParticleRenderer::gpuSetup(std::shared_ptr<Program> prog, int numP) {
	//make this lookup table at setup to save render computation time
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 5; j++) {
			SpriteRowColumnTable.push_back(make_pair(i, j));
		}
	}
	for (int i = 0; i < ParticleRenderer::numUniqueBufObjs; i++) {
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


void ParticleRenderer::drawSplash(float totalTime) {
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
	mat4 Model = glm::translate(mat4(1.0f), trans->GetPos());
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model));
	glUniform1f(prog->getUniform("totalTime"), totalTime);
	glUniform3f(prog->getUniform("centerPos"), trans->GetPos().x, trans->GetPos().y, trans->GetPos().z);
	glUniform1f(prog->getUniform("alphaMult"), clamp(1 - totalTime, 0.0f, 1.0f));
	
	// Draw the points
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArraysInstanced(GL_POINTS, 0, 1, numP);
	glDisable(GL_BLEND);
	prog->unbind();
}

void ParticleRenderer::drawSand(float totalTime) {
	prog->bind();
	glPointSize(pointSize);
	glBindVertexArray(vertArrObj[bufObjIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, colBufObj[bufObjIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObj[bufObjIndex]);
	glBindBuffer(GL_ARRAY_BUFFER, rotBufObj[bufObjIndex]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, glm::value_ptr(View));
	mat4 Model = trans->GetModelMat();
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model));
	glUniform1f(prog->getUniform("totalTime"), totalTime);


	trans->ApplyTranslation(calcNewPos(vec3(1, 0, 0), frametime));

	glUniform3fv(prog->getUniform("centerPos"), 1, glm::value_ptr(trans->GetPos()));

	//do the calculation for, based on the time, which row/column images should be used.
	//over a period of 2s, so map [0-2) to [0-39], technically [0-40) first.
	int spriteNum = std::min(floor(totalTime * 10),39.0f); //an extra frametime is added for update. Make sure it doesn't mess up indexing.
	//get the next and previous images, to potentially do some blending.
	int spriteNumPrev = (spriteNum - 1) % 40;
	if (spriteNumPrev == -1) spriteNumPrev = 39;
	int spriteNumNext = (spriteNum + 1) % 40;
	int spriteNums[3] = { spriteNumPrev, spriteNum, spriteNumNext };
	int rows[3];
	int cols[3];
	//lookup the row and column of each.
	for (int i = 0; i < 3; i++) {
		rows[i] = SpriteRowColumnTable[spriteNums[i]].first;
		cols[i] = SpriteRowColumnTable[spriteNums[i]].second;
	}
	glUniform3iv(prog->getUniform("Row"), 1, rows);
	glUniform3iv(prog->getUniform("Column"), 1, cols);
	vec3 campos = Camera::GetInstance(vec3(0, 1, 0)).GetPos();
	glUniform3f(prog->getUniform("campos"), campos.x, campos.y, campos.z);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//instead of drawing the entire array, just start from a random location to get a random set of data to work with. numP should be less than the max particles allocated in gpusetup.
	glDrawArraysInstanced(GL_POINTS, startIndex, 1, numP);
	glDisable(GL_BLEND);
	glPointSize(originalPointSize);
	prog->unbind();
}

void ParticleRenderer::drawSmoke(float totalTime) {
	prog->bind();

	prog->unbind();
}

vec3 ParticleRenderer::calcNewPos(vec3 globalWindVec, float frametime) {
	constexpr float horizontalSpread = 10.0f;
	vec3 offset = vec3(0, 0, 0);// vec3(horizontalSpread * pointRotations[bufObjIndex][startIndex], 0, horizontalSpread * pointRotations[bufObjIndex][startIndex + 2]);


	vec3 globalWindForce = totalTime * globalWindVec;
	vec3 individualWindForce = totalTime * vec3(pointRotations[bufObjIndex][startIndex], pointRotations[bufObjIndex][startIndex + 1], pointRotations[bufObjIndex][startIndex + 2]);
	vec3 forward = Player::GetInstance(vec3(0)).GetForward();
	float speed = Player::GetInstance(vec3(0)).GetCurrentSpeed();
	offset += (1.0f * globalWindForce) + (1.0f * individualWindForce) + (-50.0f * forward * speed);
	//newPosition.y = heightCalc(newPosition.x, newPosition.z) - 15.0f;
	return offset * frametime;
}