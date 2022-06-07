#pragma once
#include "Collision.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;
class CollisionNode {
    void Update(float frameTime, ComponentManager* compMan);
    void constructBoundingSphere();
    bool valid = false;
    vec3 pos;
    float radius;
    shared_ptr<CollisionNode> parent;
    shared_ptr<Collision> self;
    vector<shared_ptr<CollisionNode>> children;
};