#include "ParticleStaticSplash.h"
#include "ComponentManager.h"
void ParticleStaticSplashRenderer::Update(float frameTime, ComponentManager* compMan)
{
    //reset the particle orientation to the camera's view matrix.
    particles->setCamera(compMan->GetCamera().GetView());
    Draw(frameTime);
}

void ParticleStaticSplashRenderer::Draw(float frameTime)
{
    //pos = trans->GetPos();
    particles->update();
    particles->drawMe(prog);
}

void ParticleStaticSplashRenderer::Init(ComponentManager* compMan)
{
    //link transform component
    GameObject obj = compMan->GetGameObject(Name);
    int index = obj.GetComponentLocation("Transform");
    trans = static_pointer_cast<Transform>(compMan->GetComponent("Transform", index));

    //might be useful. Get the time at which this init happened.
    startTime = compMan->GetGameState()->GetTotalFrameTime();

    //set the particles' starting position to the linked transfor component
    particles = make_unique<particleSys>(trans->GetPos());
    //set up gpu data
    particles->gpuSetup();
}
