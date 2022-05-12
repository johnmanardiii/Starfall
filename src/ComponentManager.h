#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Component.h"
#include "Movement.h"
#include "Player.h"
#include "Collect.h"
#include "Transform.h"
#include "Collision.h"
#include "TextureRenderer.h"
#include "StarRenderer.h"
#include "ParticleStaticSplash.h"
#include "RandomGenerator.h"
#include "GameState.h"
#include "HeightCalc.h"

#include <glm/gtx/quaternion.hpp>
#include <string>
#include <memory>
#include <map> // uses red-black tree, guarantees ordering of keys
#include <unordered_map> //uses hashtable
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
    ComponentManager();
    //from the name, gets a way to access all the object's data members.
    GameObject GetGameObject(string name);
    //anything the component manager needs to do BEFORE frame-by-frame operations
    void Init(std::string resourceDirectory);
    
    void AddLineOfStars();

    void AddBunchOfSandParticles();

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
    Player& GetPlayer() { return player; }
    GameState* GetGameState() { return &state; }
    
private:
    //helper functions to differentiate parts of AddGameObject.
    pair<string, size_t> addToComponentList(const shared_ptr<Component>& comp);
    int getNextOpenSlot(OpenSlots& slots);
    template<typename CONCRETE, typename ABSTRACT>
    void addHelper(shared_ptr<CONCRETE> comp, vector<shared_ptr<ABSTRACT>>& compList, int& index);

    //the objects
    unordered_map<string, GameObject> objects;
    
    //something to hold game state information.
    GameState state = GameState(30, this);

    //the various components
    unordered_map <string, vector<shared_ptr<Component>>> components;
    unordered_map <string, OpenSlots> componentOpenSlots;
    //one camera for now
    Camera& camera = Camera::GetInstance(vec3(0,1,0));
    Player& player = Player::GetInstance(vec3(0, 1, 0));
};

