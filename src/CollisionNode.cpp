#include "CollisionNode.h"

void CollisionNode::Update(float frameTime, ComponentManager* compMan) {
    if (children.empty()) {
        if (!self.get()) {//if you both have no children (they were removed) and don't contain a Collision object, your container is marked for garbage collection
            isKilled = true;
        }
        self->Update(frameTime, compMan); //update yourself. You resolve to an actual Collision object
    }
    else {//You aren't a collision object, but you have children that could be.
        for (auto& col : children) {
            col->Update(frameTime, compMan);
        }
    }
}

//go down the hierarchy, and compute a bounding sphere, that encompasses all of the bounding spheres of your children.
//if the child's bounding sphere is not valid (it hasn't been computed, or the objects moved or something, recursively generate it.
void CollisionNode::constructBoundingSphere()
{
    pos = vec3(0);
    if (children.empty()) {
        pos = self->getCenter();
        radius = self->getRadius();
        valid = true;
        return; //exit early, no children so we are done.
    }
    vec3 minPos = vec3(FLT_MAX), maxPos = vec3(-FLT_MAX);
    for (auto& col : children) {
        //if bounding sphere hasn't been computed for the child, compute/retrieve it first.
        if (!col->valid) {
            col->constructBoundingSphere();
        }
        //get min vals
        if (col->pos.x < minPos.x) minPos.x = col->pos.x;
        if (col->pos.y < minPos.y) minPos.y = col->pos.y;
        if (col->pos.z < minPos.z) minPos.z = col->pos.z;
        //get max vals
        if (col->pos.x > maxPos.x) maxPos.x = col->pos.x;
        if (col->pos.y > maxPos.y) maxPos.y = col->pos.y;
        if (col->pos.z > maxPos.z) maxPos.z = col->pos.z;
    }
    pos = (minPos + maxPos) / 2.0f;

    //we now have a center of all collision centers. Find the farthest center away and retrieve it, and its radius.
    float farthestPosDistance = 0;
    float farthestRadius = 0;
    for (const auto& col : children) {
        float posDistance = length(col->pos - this->pos); //explicit "this" for readability
        if (posDistance > farthestPosDistance) {
            farthestPosDistance = posDistance;
            farthestRadius = col->radius;
        }
    }
    //we now have a center, and a center+radius for the farthest-away collision node in this group.
    //can now compute the total bounding radius.
    radius = farthestPosDistance + farthestRadius;
    valid = true; //we have computed our bounding sphere for this node.
}
