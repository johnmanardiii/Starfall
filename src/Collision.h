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
    void Resolve(shared_ptr<Collision> other, float frameTime);
    void updateBasedOnCollision(glm::vec3 collisionDirection, float frameTime);
    void Update(float frameTime, ComponentManager* compMan);
    bool IsCollected() const { return hasBeenTouchedByCamera; }
    float getRadius();
private:
    glm::vec3 getCenterOfBBox();
    void collideWithGroundPlane(float frameTime, ComponentManager* compMan);
    void collideWithCamera(float frameTime, ComponentManager* compMan);

    bool hasBeenTouchedByCamera = false;
    Shape shape;
    shared_ptr<Movement> movement;
    shared_ptr<Transform> transform;
};