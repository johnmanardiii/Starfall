#pragma once
#include <string>
#include <memory>

#include "Renderer.h"
#include "Program.h"
#include "particleSys.h"
#include "ShaderManager.h"
class ComponentManager;

class ParticleStaticSplashRenderer : public Renderer
{
public:
    ParticleStaticSplashRenderer(std::string tex, std::string gameObjectName) 
        : Renderer(gameObjectName) {
        ShaderManager shaderMan = ShaderManager::GetInstance();
        prog = shaderMan.GetShader("particle");
        texture = shaderMan.GetTexture(tex);
    }
    void Draw(float frameTime);
    void Update(float frameTime, ComponentManager* compMan);
    void Init(ComponentManager* compMan);


    std::unique_ptr<particleSys> particles;
    

private:
    float startTime = 0;
    GLuint texture;
    
};