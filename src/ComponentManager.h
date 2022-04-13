#pragma once

#include "GameObject.h"
#include "Camera.h"
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <queue>
using std::string;
using std::map;
using std::vector;
using std::shared_ptr;
using std::priority_queue;

class ComponentManager
{
    //a queue that is automatically sorted, with smaller elements at the top.
    //using typedef to localize this long definition.
    typedef priority_queue<size_t, vector<size_t>, std::greater<size_t>> OpenSlots;
public:
    void DeleteObject(GameObject obj);
    GameObject GetObject(string name);
    //anything the component manager needs to do BEFORE frame-by-frame operations
    void Init();
    
    //update components
    void UpdateComponents();
    GameObject AddGameObject(string name, vector<shared_ptr<Component>> comps);
    void RemoveGameObject(string name);
    //anything the component manager needs to do AFTER frame-by-frame operations
    void Cleanup();
    Camera& GetCamera() { return camera; } //direct access, camera isn't componentized yet.
private:
    //the objects
    map<string, GameObject> objects;

    //the various components
    
    //one camera for now
    Camera& camera = Camera::GetInstance();

    //Renderer
    vector<Component> renderers; //TODO change type to Renderer
    OpenSlots rendererSlots;
    //Movement
    vector<Component> movements; //TODO change type to movement component name
    OpenSlots movementSlots;
    //Transform
    vector<Component> transforms;
    OpenSlots transformSlots;
    //Collision
    vector<Component> collisions; //TODO change type to collision, or bounding sphere/box.
    OpenSlots collisionSlots;
    //Spawner
    //TODO write singleton for spawner.
};

