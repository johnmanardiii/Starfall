#include "Collision.h"

glm::vec3 Collision::getCenterOfBBox(Shape s){
    return glm::vec3(
        (s.max.x + s.min.x) / 2,
        (s.max.y + s.min.y) / 2,
        (s.max.z + s.min.z) / 2
    );
}

void Collision::Init(ComponentManager& manager){
    GameObject obj = manager.GetObject(Name);
    size_t transformIndex = obj.GetComponentLocation("Transform");
    size_t movementIndex = obj.GetComponentLocation("Movement");

    movement = static_pointer_cast<Movement>(manager.GetComponent("Movement", movementIndex));
    transform = static_pointer_cast<Transform>(manager.GetComponent("Transform", transformIndex));
}