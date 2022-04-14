#pragma once

#include <glm/glm.hpp>

#include "Component.h"
#include "Movement.h"
#include "Shape.h"

class Collision : public Component {
public:
    Collision(Shape s) : shape(s) {}
    void Init(ComponentManager& manager);
    Shape shape;
    glm::vec3 getCenterOfBBox(Shape s);
private:
    shared_ptr<Movement> movement;
    shared_ptr<Transform> transform;
};