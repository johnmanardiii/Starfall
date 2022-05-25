#pragma once
#include <string>
#include <memory>
#include "GLSL.h"
#include "Renderer.h"
#include "Program.h"
#include "ShaderManager.h"
class ComponentManager;

class ParticleRenderer : public Renderer
{
public:
    ParticleRenderer(std::string tex, std::string shaderName, std::string gameObjectName, int numParticles, void(ParticleRenderer::*drawFunc)(float))
        : Renderer(gameObjectName) {
        func = drawFunc;
        ShaderManager shaderMan = ShaderManager::GetInstance();
        prog = shaderMan.GetShader(shaderName);
        texture = shaderMan.GetTexture(tex);

        numP = numParticles;

        View = glm::mat4(1.0);
        bufObjIndex = ParticleRenderer::currBufObjs % numUniqueBufObjs;
        ParticleRenderer::currBufObjs++;

    }
    void Draw(float frameTime);
    void Update(float frameTime, ComponentManager* compMan);
    void Init(ComponentManager* compMan);
    
    float totalTime = 0;
    constexpr static int numUniqueBufObjs = 7;
    static int currBufObjs;
    static vector<vector<float>> pointColors;
    static vector<vector<float>> pointNormals;
    static vector<vector<float>> pointRotations;

    static vector<unsigned> vertArrObj;
    static vector<unsigned> colBufObj;
    static vector<unsigned> norBufObj;
    static vector<unsigned> rotBufObj;

    void drawSplash(float totalTime);
    void drawSand(float totalTime);
    void drawSmoke(float totalTime);
    static void gpuSetup(std::shared_ptr<Program> prog, int numP);
    void setCamera(mat4 inC) { View = inC; }
    void setProjection(mat4 inP) { Projection = inP; }


private:
    void(ParticleRenderer::*func)(float totalTime);
    GLuint texture;
    
    int numP;
    vec3 start;

    int bufObjIndex;

    mat4 View;
    mat4 Projection;
};