#pragma once

#include "GameObject.h"
#include "Camera.h"
#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;

class ComponentManager
{
public:
    void deleteObject(GameObject obj);
    GameObject getObject(string name);
    //anything the component manager needs to do BEFORE frame-by-frame operations
    void init();
    
    //update components
    void updateComponents();

    //anything the component manager needs to do AFTER frame-by-frame operations
    void cleanup();
    Camera& getCamera() { return camera; } //direct access, camera isn't componentized yet.
private:
    //the objects
    map<string, GameObject> objects;

    //the various components
    
    //one camera for now
    Camera& camera = Camera::getInstance();
};
