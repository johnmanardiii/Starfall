#pragma once
#include <string>
#include <memory>
#include "GLSL.h"
#include "Renderer.h"
#include "Program.h"
#include "ShaderManager.h"
#include <glm/gtx/matrix_decompose.hpp>

class ComponentManager;



class ParticleRenderer : public Renderer
{
public:
    ParticleRenderer(std::string tex, std::string shaderName, std::string gameObjectName, int numParticles, float pSize, float cullRadius, void(ParticleRenderer::*drawFunc)(float))
        : Renderer(gameObjectName) {
        func = drawFunc;
        ShaderManager shaderMan = ShaderManager::GetInstance();
        prog = shaderMan.GetShader(shaderName);
        texture = shaderMan.GetTexture(tex);
        numP = numParticles;
        cullingRadius = cullRadius;
        pointSize = pSize;
        View = glm::mat4(1.0);
        startIndex = (numP == 20000) ? 0 : rand() % (20000 - numP);
        bufObjIndex = ParticleRenderer::currBufObjs % numUniqueBufObjs;
        ParticleRenderer::currBufObjs++;
    }
    void Draw(float frameTime);
    void Update(float frameTime, ComponentManager* compMan);
    void Init(ComponentManager* compMan);
    
    float totalTime = 0;
    int startIndex;
    static int currBufObjs;
    static vector<vector<float>> pointColors;
    static vector<vector<float>> pointNormals;
    static vector<vector<float>> pointRotations;
    static vector<pair<int, int>> SpriteRowColumnTable;

    constexpr static int numUniqueBufObjs = 7;
    static vector<unsigned> vertArrObj;
    static vector<unsigned> colBufObj;
    static vector<unsigned> norBufObj;
    static vector<unsigned> rotBufObj;

    void drawSplash(float totalTime);
    void drawSand(float totalTime);
    void drawSmoke(float totalTime);
    vec3 calcNewPos(vec3 globalWindVec, float frametime);
    static void gpuSetup(std::shared_ptr<Program> prog, int numP);
    void setCamera(mat4 inC) { View = inC; }
    void setProjection(mat4 inP) { Projection = inP; }

    constexpr static float originalPointSize = 5.0f;
    bool sorted = false; //particles are not sorted by default. Sort them if you are using transparent alpha values.
    const glm::vec3& getPos() { return trans->GetPos(); }
    int bufObjIndex;
private:
    void(ParticleRenderer::*func)(float totalTime);
    GLuint texture;
    float pointSize;
    int numP;
    vec3 start;
    float frametime;
    mat4 View;
    mat4 Projection;
};



class ParticleSorter {
public:
    bool operator()(const std::shared_ptr<Component>& c0, const std::shared_ptr<Component>& c1) const
    {
        auto& p0 = static_pointer_cast<ParticleRenderer>(c0);
        auto& p1 = static_pointer_cast<ParticleRenderer>(c1);
        const glm::vec3& x0 = p0->getPos();
        const glm::vec3& x1 = p1->getPos();
        
        constexpr glm::vec3 globalWindDir = vec3(1, 0, 0);
        
        const float t0 = p0->totalTime;
        const float t1 = p1->totalTime;

        vec3 curPos0 = (6 * t0 * globalWindDir) + (8 * t0 * ParticleRenderer::pointRotations[p0->bufObjIndex][p0->startIndex]);
        vec3 curPos1 = (6 * t1 * globalWindDir) + (8 * t1 * ParticleRenderer::pointRotations[p1->bufObjIndex][p1->startIndex]);
        vec3 campos = Camera::GetInstance(vec3(0, 1, 0)).GetPos();
        

        return (glm::distance(curPos0, campos) < glm::distance(curPos1, campos));
        //return x0w.z < x1w.z;
    }
    mat4 C; // current camera matrix
};
