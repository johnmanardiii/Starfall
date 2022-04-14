#pragma once

#include <glm/glm.hpp>

#include "Component.h"
#include "Movement.h"
#include "Transform.h"
#include "Shape.h"

class Collision : public Component {
public:
    Collision(std::string name, Shape s) : Component(name), shape(s) {}
    void Init(ComponentManager& manager);
    Shape shape;
    glm::vec3 getCenterOfBBox(Shape s);
private:
    shared_ptr<Movement> movement;
    shared_ptr<Transform> transform;
};