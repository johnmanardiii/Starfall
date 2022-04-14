#pragma once

#include <glm/glm.hpp>
#include <algorithm>

#include "Component.h"
#include "Movement.h"
#include "Transform.h"
#include "Shape.h"

class Collision : public Component {
public:
    Collision(std::string name, Shape s) : Component(name), shape(s) {}
    void Init(ComponentManager& manager);
    void Resolve(Collision& other, float frameTime);
    void updateBasedOnCollision(vec3 collisionDirection, float frameTime);
    void Update(float frameTime) { throw std::runtime_error("don't use this, use resolve"); }
private:
    glm::vec3 getCenterOfBBox();
    float getRadius();

    Shape shape;
    shared_ptr<Movement> movement;
    shared_ptr<Transform> transform;
};