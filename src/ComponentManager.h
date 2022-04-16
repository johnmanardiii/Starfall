#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Component.h"
#include "Movement.h"
#include "Collect.h"
#include "Transform.h"
#include "Collision.h"
#include "TextureRenderer.h"
#include "RandomGenerator.h"
#include "GameState.h"

#include <glm/gtx/quaternion.hpp>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;

class ComponentManager
{
    //a queue that is automatically sorted, with smaller elements at the top.
    //using typedef to localize this long definition.
    typedef priority_queue<size_t, vector<size_t>, std::greater<size_t>> OpenSlots;
public:
    
    //from the name, gets a way to access all the object's data members.
    GameObject GetGameObject(string name);
    //anything the component manager needs to do BEFORE frame-by-frame operations
    void Init(std::string resourceDirectory);
    
    //update components. Done once every render pass.
    void UpdateComponents(float frameTime, int width, int height);
    //Make components, throw them in a vector<shared_ptr>> in any order, give it a name, and componentManager will manage it.
    void AddGameObject(string name, vector<shared_ptr<Component>> comps);
    //deactivate object's components, free up their space, and remove the GameObject abstraction, completing the delete.
    void RemoveGameObject(string name);
    //anything the component manager needs to do AFTER frame-by-frame operations. Currently nothing.
    void Cleanup();
    //useful if components of a game object need to grab references to each other.
    shared_ptr<Component> GetComponent(string, int);
    Camera& GetCamera() { return camera; } //direct access, camera isn't componentized yet.
    GameState* GetGameState() { return &state; }
    
private:
    //helper functions to differentiate parts of AddGameObject.
    pair<string, size_t> addToComponentList(const shared_ptr<Component>& comp);
    int getNextOpenSlot(OpenSlots& slots);
    template<typename T>
    void addHelper(shared_ptr<T> comp, vector<shared_ptr<T>>& compList, int& index);

    //the objects
    map<string, GameObject> objects;
    
    //something to hold game state information.
    GameState state = GameState(100, this);

    //the various components
    //one camera for now
    Camera& camera = Camera::GetInstance(vec3(0,1,0));

    vector<string> componentVectorNames{ "Movement", "Transform", "Collision", "Renderer", "Collect" }; //MTCRC
    //Movement
    vector< shared_ptr<Movement>> movements; //TODO change type to movement component name
    OpenSlots movementSlots;
    //Transform
    vector< shared_ptr<Transform>> transforms; //TODO change type to transforms component name
    OpenSlots transformSlots;
    //Renderer
    vector<shared_ptr<Renderer>> renderers; //TODO change type to Renderer
    OpenSlots rendererSlots;
    //Collision
    vector<shared_ptr<Collision>> collisions; //TODO change type to collision, or bounding sphere/box.
    OpenSlots collisionSlots;
    //Collect
    vector<shared_ptr<Collect>> collects; //TODO change type to collision, or bounding sphere/box.
    OpenSlots collectSlots;
};

