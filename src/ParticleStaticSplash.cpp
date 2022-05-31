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

vector<pair<float, float>>  ParticleRenderer::SpriteRowColumnTable;


float randFloat(float l, float h)
{
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * l + r * h;
}


void ParticleRenderer::Update(float frameTime, ComponentManager* compMan)
{   
	
    totalTime += frameTime;
	this->frametime = frameTime;
	if (totalTime > LIFETIME) {
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
	//constants used for texturing the specific sprite sheet.
//total image width and height in pixels
	const float IMAGE_WIDTH_PIX = 1024, IMAGE_HEIGHT_PIX = 1024; //truly perfect numbers
	//width and height of a single sprite in pixels
	const float SPRITE_WIDTH_PIX = 128, SPRITE_HEIGHT_PIX = 128;
	//horizontal and vertical spacing that exists inbetween each sprite
	const float SPRITE_HORIZONTAL_SPACING = 0, SPRITE_VERTICAL_SPACING = 0;
	//texture space offsets.
	const float WIDTH_OFF_TEX = (SPRITE_WIDTH_PIX) / IMAGE_WIDTH_PIX;
	const float HEIGHT_OFF_TEX = (SPRITE_HEIGHT_PIX) / IMAGE_HEIGHT_PIX;

	//make this lookup table at setup to save render computation time
	for (int rows = 0; rows < 8; rows++) {
		for (int cols = 0; cols < 8; cols++)
			SpriteRowColumnTable.push_back(make_pair(rows * HEIGHT_OFF_TEX, cols * WIDTH_OFF_TEX));
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
	float alphaTime = 0;
	if (totalTime < 1) {
		alphaTime = totalTime;
	}
	else if (totalTime < LIFETIME - 1) {
		alphaTime = 1;
	}
	else {
		alphaTime = LIFETIME - totalTime;
	}
	glUniform1f(prog->getUniform("alphaTime"), alphaTime);
	vec4 nearPlane = Camera::GetInstance(vec3()).getVFCPlanes()[4];
	
	trans->SetPos(Camera::GetInstance(vec3()).GetPos() + 15.0f * vec3(nearPlane) + vec3(0, 5, 0));
	glUniform3fv(prog->getUniform("centerPos"), 1, glm::value_ptr(trans->GetPos()));

	//do the calculation for, based on the time, which row/column images should be used.
	//over a period of 2s, so map [0-2) to [0-39], technically [0-40) first.
	int spriteNum = int(totalTime * 60.0f) % 64; //an extra frametime is added for update. Make sure it doesn't mess up indexing.
	//get the next and previous images, to potentially do some blending.
	pair<vec3, vec3> coords = calcSpritePos(spriteNum);
	
	glUniform3fv(prog->getUniform("Row"), 1, value_ptr(coords.first));
	glUniform3fv(prog->getUniform("Column"), 1, value_ptr(coords.second));
	//cout << "telling GPU to render current sprite image: " << coords.first.y << " - " << coords.second.y << endl;
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
	offset += (1.0f * globalWindForce) + (1.0f * individualWindForce);
	//newPosition.y = heightCalc(newPosition.x, newPosition.z) - 15.0f;
	return offset * frametime;
}

//given a number in range 0-39, output the texcoords that will be used for the previous, current, and next frames.
pair<vec3, vec3> ParticleRenderer::calcSpritePos(int curr) {
	int prev = (curr - 1) % 64;
	if (prev == -1) prev = 63;
	int next = (curr + 1) % 64;
	
	vec3 rows = vec3(SpriteRowColumnTable[prev].first, SpriteRowColumnTable[curr].first, SpriteRowColumnTable[next].first);
	vec3 cols = vec3(SpriteRowColumnTable[prev].second, SpriteRowColumnTable[curr].second, SpriteRowColumnTable[next].second);
	//lookup the row and column of each. Think about adding all the above to lookup table as well
	return make_pair(rows, cols);
}