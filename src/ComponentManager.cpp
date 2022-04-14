#include "ComponentManager.h"

void ComponentManager::DeleteObject(GameObject obj)
{
    // TODO removes all components specified in a Game Object's object list. 
    // e.x. gameobject with (0, -1, 1) removes first component, first position, and third component, second position.
    // just realized that references will change if positions are shifted around.
}

GameObject ComponentManager::GetObject(string name)
{
    //TODO this should return a reference/copy (discuss) to a game object
    //based on the key in a map.
    auto it = objects.find(name);
    if (it == objects.end()) {
        std::cerr << "GetObject with name: " << name << " was not found in objects list." << std::endl;
        exit(1);
    }
    return it->second;
}

shared_ptr<Component> ComponentManager::GetComponent(string compName, int index)
{
    if (compName == componentVectorNames[0]) { //movement
        shared_ptr<Movement> copyPtr = movements[index];
        return copyPtr;
    }
    else if (compName == componentVectorNames[1]) { //transform
        shared_ptr<Transform> copyPtr = transforms[index];
        return copyPtr;
    }
     /*TODO re - add references later
    else if (compName == componentVectorNames[2]) { //collision
        return make_shared<Collision>(&collisions[index]);
    }
    else if (compName == componentVectorNames[3]) { //render
        return make_shared<Render>(&renderers[index]);
    }
    */
}

void ComponentManager::Init()
{
    
    camera = Camera::GetInstance(vec3(0,1,0));
    //TODO get real data in here, about starting information of components.
    

    //declaration style 1
    
    //quick test. Add 9 bunnies, then delete bunny0, bunny4, and bunny8. 
    //all other slots should be active, and there should be 6 remaining.
    
    for (int i = 0; i < 9; i++) {
        shared_ptr<Component> renderer = nullptr;
        shared_ptr<Component> movement = make_shared<Movement>("bunny" + to_string(i));
        shared_ptr<Component> transform = make_shared<Transform>("bunny" + to_string(i));
        shared_ptr<Component> collision = nullptr;
        vector<shared_ptr<Component>> Bunny = { renderer, movement, transform, collision };
        AddGameObject("bunny" + to_string(i), Bunny);
    }
    //delete 3
    RemoveGameObject("bunny0");
    RemoveGameObject("bunny4");
    RemoveGameObject("bunny8");
    //add 1 back
    shared_ptr<Component> renderer = nullptr;
    shared_ptr<Component> movement = make_shared<Movement>("bunny9");
    shared_ptr<Component> transform = make_shared<Transform>("bunny");
    shared_ptr<Component> collision = nullptr;
    vector<shared_ptr<Component>> Bunny = { renderer, movement, transform, collision };
    AddGameObject("bunny9", Bunny);
    
    //declaration style 2, with no movement component.
    
}

void ComponentManager::UpdateComponents()
{

    camera.Update();
}

void ComponentManager::AddGameObject(string name, vector<shared_ptr<Component>> comps)
{
    map<type_info*, size_t> componentList;
    //don't care what container is used to pass in components,
    //Pad unused components with null.
    map<string, size_t> objComps;
    for (const auto& comp : comps) {
        if (!comp) continue; //null check
        //if not null
        auto p = addToComponentList(comp);
        objComps.insert(p);
    }
    objects[name] = GameObject(name, objComps);
    auto locations = objects[name].GetComponentLocations();
    for (auto comp : locations) {
        auto name = comp.first;
        auto index = comp.second;

        //free up for insertion. Do this by supplying the component's
        //indices for use by a new component, and marking the component as not in use.
        if (name == componentVectorNames[0]) { //movement
            movements[index]->Init(*this);
        }
        else if (name == componentVectorNames[1]) { //transform
            transforms[index]->Init(*this);
        } 
        else if (name == componentVectorNames[2]) { //collision
            collisions[index]->Init(*this);
        }
        else if (name == componentVectorNames[3]) { //render
            renderers[index]->Init(*this);
        }
        //TODO add additional potential components.
    }//end processing component vector freeing
}

//add the component to the first-available position of the corresponding vector of components.
pair<string, size_t> ComponentManager::addToComponentList(const shared_ptr<Component>& comp) {
    //these series of checks check whether an attempted cast returns a nullptr, indicating that
    //a cast could/couldn't take place, effectively type-checking the object.
    int index = -1; //if this remains at -1 there wasn't a successful cast.
    string compType = "undefinedComponentType"; //make it really obvious if not detected.
    if (auto ptr = dynamic_pointer_cast<Transform>(comp)) {
        index = getNextOpenSlot(transformSlots);
        compType = componentVectorNames[1];
        addHelper(ptr, transforms, index);
    }
    else if (auto ptr = dynamic_pointer_cast<Movement>(comp)) {
        index = getNextOpenSlot(movementSlots);
        compType = componentVectorNames[0];
        addHelper(ptr, movements, index);
    }
    //TODO the other concrete types. Format should be pretty much identical.

    // finally(this should happen at the end, unconditionally, for all component types :
    //return the index where the component resides now.
    return make_pair(compType, index); //insert this into the GameObject.
}

//This modifies compList by inserting comp.
template<typename T>
void ComponentManager::addHelper(shared_ptr<T> comp, vector<shared_ptr<T>>& compList, int& index) {
    if (index == -1) {
        index = static_cast<int>(compList.size());
        compList.push_back(comp);
    }
    else {
        compList[index] = comp;
    }
}

//returns the index of the next open slot, or -1 if there are no open slots.
int ComponentManager::getNextOpenSlot(OpenSlots& slots) {
    //this indicates that there are either no available slots in an empty vector,
    //or there are no empty slots in a vector with things in it.
    //Either way the correct response is push_back.
    if (slots.empty()) return -1;
    else {
        size_t val = slots.top();
        slots.pop(); //remove the element, as the corresponding position is going to be immediately filled.
        return val;
    }
}

void ComponentManager::RemoveGameObject(string name)
{
    //make a copy, don't actually remove from map until its components are all gone
    GameObject obj = GetObject(name);
    assert(name == obj.Name); //DEBUG quick sanity check
    map<string, size_t> comps = obj.GetComponentLocations();
    for (auto comp : comps) {
        auto name = comp.first;
        auto index = comp.second;

        //free up for insertion. Do this by supplying the component's
        //indices for use by a new component, and marking the component as not in use.
        if (name == componentVectorNames[0]) { //movement
            movements[index]->IsActive = false;
            movementSlots.push(index); 
        }
        else if (name == componentVectorNames[1]) { //transform
            movements[index]->IsActive = false;
            transformSlots.push(index);
        } 
        else if (name == componentVectorNames[2]) { //collision
            movements[index]->IsActive = false;
            collisionSlots.push(index);
        }
        else if (name == componentVectorNames[3]) { //render
            movements[index]->IsActive = false;
            rendererSlots.push(index);
        }
        //TODO add additional potential components.
    }//end processing component vector freeing

    //no longer referenced anywhere, delete from map.
    objects.erase(name); 
}

void ComponentManager::Cleanup()
{
}
