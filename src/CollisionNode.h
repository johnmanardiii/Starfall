#pragma once
#include "Collision.h"
#include <glm/glm.hpp>
#include <memory>

using namespace std;
using namespace glm;
class CollisionNode {
public:
    CollisionNode(){}
    CollisionNode(shared_ptr<Collision> selfCol) : self(selfCol) {}
    void Update(float frameTime, ComponentManager* compMan);
    void constructBoundingSphere();
    shared_ptr<CollisionNode> parent;
    shared_ptr<Collision> self;
    vector<shared_ptr<CollisionNode>> children;
private:
    bool isKilled = false;
    bool valid = false;
    vec3 pos;
    float radius;
    
};