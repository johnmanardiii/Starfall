#pragma once

#include <glm/glm.hpp>
#include <algorithm>
#include <memory>
#include <stdexcept>

#include "ShaderManager.h"

#include "Component.h"

#include "Shape.h"

using namespace std;
class Collision : public Component {
public:
    Collision(std::string name, string model) : Component(name)  {
        ShaderManager shaderMan = ShaderManager::GetInstance();
        shape = *shaderMan.GetModel(model);
    }
    void Init(ComponentManager* manager);
    
    void Update(float frameTime, ComponentManager* compMan);
    bool IsCollected() const { return hasBeenTouchedByPlayer; }
    float getRadius();
private:
    glm::vec3 getCenterOfBBox();
    void collideWithPlayer(float frameTime, ComponentManager* compMan);
    float maxDistance = 200;
    bool hasBeenTouchedByPlayer = false;
    Shape shape;
    shared_ptr<Transform> transform;
};