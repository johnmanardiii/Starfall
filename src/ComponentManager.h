#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Component.h"
#include "Movement.h"
#include "Transform.h"

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

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
    void AddGameObject(string name, vector<shared_ptr<Component>> comps);
    void RemoveGameObject(string name);
    //anything the component manager needs to do AFTER frame-by-frame operations
    void Cleanup();
    shared_ptr<Component> GetComponent(string, int);
    Camera& GetCamera() { return camera; } //direct access, camera isn't componentized yet.
private:
    //the objects
    map<string, GameObject> objects;
    
    //helper functions to differentiate parts of AddGameObject.
    pair<string, size_t> addToComponentList(const shared_ptr<Component>& comp);
    int getNextOpenSlot(OpenSlots& slots);
    template<typename T>
    void addHelper(shared_ptr<T> comp, vector<shared_ptr<T>>& compList, int& index);

    //the various components
    //one camera for now
    Camera& camera = Camera::GetInstance();

    vector<string> componentVectorNames{ "Movement", "Transform", "Collision", "Render" };
    //Renderer
    vector<shared_ptr<Component>> renderers; //TODO change type to Renderer
    OpenSlots rendererSlots;
    //Movement
    vector< shared_ptr<Movement>> movements; //TODO change type to movement component name
    OpenSlots movementSlots;
    //Transform
    vector< shared_ptr<Transform>> transforms; //TODO change type to transforms component name
    OpenSlots transformSlots;
    //Collision
    vector< shared_ptr<Component>> collisions; //TODO change type to collision, or bounding sphere/box.
    OpenSlots collisionSlots;
    
    //Spawner
    //TODO write singleton for spawner.
};

