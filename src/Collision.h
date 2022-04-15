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
    void Init(ComponentManager& manager);
    void Resolve(shared_ptr<Collision> other, float frameTime);
    void updateBasedOnCollision(glm::vec3 collisionDirection, float frameTime);
    void Update(float frameTime, ComponentManager& compMan) { throw std::runtime_error("don't use this, use resolve"); }
private:
    glm::vec3 getCenterOfBBox();
    float getRadius();

    Shape shape;
    shared_ptr<Movement> movement;
    shared_ptr<Transform> transform;
};