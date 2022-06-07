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

glm::vec3 Collision::getCenter()
{
   return transform->GetPos();
}

void Collision::Update(float frameTime, ComponentManager* compMan)
{
    collideWithPlayer(frameTime, compMan);
}

void Collision::collideWithPlayer(float frameTime, ComponentManager* compMan) {
    vec3 center = transform->GetPos();
    vec3 playerPos = compMan->GetPlayer().GetPosition();
    float radius = getRadius();
    float pRadius = compMan->GetPlayer().GetRadius();
    float distanceFromObjectToPlayer = glm::distance(center, playerPos);
    if (distanceFromObjectToPlayer <= radius + pRadius) {
        hasBeenTouchedByPlayer = true;
        compMan->GetGameState()->Collect();
    }
    else if (distanceFromObjectToPlayer > maxDistance) {
        //sometimes the trash takes itself out.
        compMan->RemoveGameObject(this->Name);
        //cout << "garbage-collected " << Name << endl;
    }
}

void Collision::Init(ComponentManager* compMan) {
    GameObject obj = compMan->GetGameObject(Name);
    size_t transformIndex = obj.GetComponentLocation("Transform");
    transform = static_pointer_cast<Transform>(compMan->GetComponent("Transform", transformIndex));
}
