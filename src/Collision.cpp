#include "Collision.h"
#include "ComponentManager.h"
glm::vec3 Collision::getCenterOfBBox(){
    return glm::vec3(
        (shape.max.x + shape.min.x) / 2,
        (shape.max.y + shape.min.y) / 2,
        (shape.max.z + shape.min.z) / 2
    );
}

float Collision::getRadius(){
    //very conservative estimate based on shape and scale.
    glm::vec3 center = getCenterOfBBox();
    glm::vec3 scale = transform->GetScale();
    float radX = (center.x - shape.min.x) * scale.x;
    float radY = (center.y - shape.min.y) * scale.y;
    float radZ = (center.z - shape.min.z) * scale.z;
    //returns the largest radius of all bounding box edges.
    //max macro expansion is dumb and never works. define lambda instead.
    auto fmax = [](float f1, float f2) {return (f1 > f2) ? f1 : f2; };
    float max1 = fmax(radX, radY); 
    float max2 = fmax(max1, radZ);

    return max2;
}

void Collision::Resolve(shared_ptr<Collision> other, float frameTime) {
    vec3 center = getCenterOfBBox();
    vec3 otherCenter = other->getCenterOfBBox();

    float radius = getRadius();
    float otherRadius = other->getRadius();

    if (glm::distance(center, otherCenter) <= radius + otherRadius) {
        vec3 collisionDirection = center - otherCenter;
        updateBasedOnCollision(collisionDirection, frameTime);
        other->updateBasedOnCollision(-collisionDirection, frameTime);
    }
}

void Collision::updateBasedOnCollision(vec3 collisionDirection, float frameTime) {
    //velocity vector is reflected.
    movement->SetVel(glm::reflect(movement->GetVel(), collisionDirection));
    //the position advances one step.
    transform->Update(frameTime);
}

void Collision::Init(ComponentManager& manager){
    GameObject obj = manager.GetObject(Name);
    size_t transformIndex = obj.GetComponentLocation("Transform");
    size_t movementIndex = obj.GetComponentLocation("Movement");

    movement = static_pointer_cast<Movement>(manager.GetComponent("Movement", movementIndex));
    transform = static_pointer_cast<Transform>(manager.GetComponent("Transform", transformIndex));
}