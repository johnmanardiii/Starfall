#include "ParticleStaticSplash.h"
#include "ComponentManager.h"
void ParticleStaticSplashRenderer::Update(float frameTime, ComponentManager* compMan)
{   
    totalTime += frameTime;
    //reset the particle orientation to the camera's view matrix.
    particles->setCamera(compMan->GetCamera().GetView());
    particles->setProjection(compMan->GetCamera().GetPerspective());
    Draw(frameTime);
}

void ParticleStaticSplashRenderer::Draw(float frameTime)
{
    particles->drawMe(prog, trans, totalTime);
    //particles->update(frameTime, trans);
}

void ParticleStaticSplashRenderer::Init(ComponentManager* compMan)
{
    //link transform component
    GameObject obj = compMan->GetGameObject(Name);
    int index = obj.GetComponentLocation("Transform");
    trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));

    //set the particles' starting position to the linked transfor component
    particles = make_unique<particleSys>(100000, trans->GetPos());
    
    
}
