#include "Collision.h"
#include "ComponentManager.h"
glm::vec3 Collision::getCenterOfBBox(){
    return glm::vec3(
        (shape.max.x + shape.min.x) / 2,
        (shape.max.y + shape.min.y) / 2,
        (shape.max.z + shape.min.z) / 2
    );
}

//this accounts for transform's scale, and returns the largest distance from center
//of an axis-aligned bounding box.
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
    vec3 center = transform->GetPos();
    vec3 otherCenter = other->transform->GetPos();

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
    movement->Move(frameTime);
}

void Collision::Update(float frameTime, ComponentManager* compMan)
{
    //hard-coded for now, this calculation will drastically change for the project.
    float g_groundSize = 20; //x-z plane stretches from -20 to +20
    float g_scale = 0.5; //scaled back, so final is -10 to +10.
    float g_edge = g_groundSize * g_scale;


    vec3 currentCenter = transform->GetPos();
    vec3 currentVelocity = movement->GetVel();
    float radius = getRadius();


    //define the two common bounds-checking operations for all directions we want to effect.
    auto lowerBound = [g_edge, radius](float center) {return center - radius < -g_edge; };
    auto upperBound = [g_edge, radius](float center) {return center + radius > g_edge; };

    //x, positive and negative
    if (lowerBound(currentCenter.x) || upperBound(currentCenter.x)) {
        movement->SetVel(vec3(-currentVelocity.x, currentVelocity.y, currentVelocity.z));
        movement->Update(frameTime, compMan); //advance one frame into the future to avoid "sticky" collisions.
    }
    currentVelocity = movement->GetVel(); //get our velocity again, as it might have changed.
    currentCenter = transform->GetPos(); //same for position.

    //y positive and negative
    //currently do nothing, the velocities in y should remain at 0.
    
    //z positive and negative
    if (lowerBound(currentCenter.z) || upperBound(currentCenter.z)) {
        movement->SetVel(vec3(currentVelocity.x, currentVelocity.y, -currentVelocity.z));
        movement->Update(frameTime, compMan); //advance one frame into the future to avoid "sticky" collisions.
    }
}

void Collision::Init(ComponentManager* compMan){
    GameObject obj = compMan->GetObject(Name);
    size_t transformIndex = obj.GetComponentLocation("Transform");
    size_t movementIndex = obj.GetComponentLocation("Movement");

    movement = static_pointer_cast<Movement>(compMan->GetComponent("Movement", movementIndex));
    transform = static_pointer_cast<Transform>(compMan->GetComponent("Transform", transformIndex));
}