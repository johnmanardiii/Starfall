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

void ComponentManager::Init()
{
    
    camera = Camera::GetInstance();
    //TODO get real data in here, about starting information of components.
    shared_ptr<Component> renderer = nullptr;
    shared_ptr<Component> movement = make_shared<Movement>();
    shared_ptr<Component> transform = make_shared<Transform>();
    shared_ptr<Component> collision = nullptr;

    //declaration style 1
    vector<shared_ptr<Component>> Bunny = { renderer, movement, transform, collision };
    AddGameObject("bunny", Bunny);
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
        objComps.insert(addToComponentList(comp));
    }
    objects[name] = GameObject(name, objComps);
}

//add the component to the first-available position of the corresponding vector of components.
pair<string, size_t> ComponentManager::addToComponentList(const shared_ptr<Component>& comp) {
    //these series of checks check whether an attempted cast returns a nullptr, indicating that
    //a cast could/couldn't take place, effectively type-checking the object.
    int index = -1; //if this remains at -1 there wasn't a successful cast.
    string compType = "undefinedComponentType"; //make it really obvious if not detected.
    if (auto ptr = dynamic_pointer_cast<Transform>(comp)) {
        index = getNextOpenSlot(transformSlots);
        compType = "Transform";
        addHelper(*ptr, transforms, index);
    }
    else if (auto ptr = dynamic_pointer_cast<Movement>(comp)) {
        index = getNextOpenSlot(transformSlots);
        compType = "Movement";
        addHelper(*ptr, movements, index);
    }
    //TODO the other concrete types. Format should be pretty much identical.

    // finally(this should happen at the end, unconditionally, for all component types :
    //return the index where the component resides now.
    return make_pair(compType, index); //insert this into the GameObject.
}

//This modifies compList by inserting comp.
template<typename T>
void ComponentManager::addHelper(T comp, vector<T>& compList, int index) {
    if (index == -1) {
        compList.push_back(comp);
    }
    else {
        compList.insert(compList.begin() + index, comp);
    }
}

//returns the index of the next open slot, or -1 if there are no open slots.
int ComponentManager::getNextOpenSlot(OpenSlots slots) {
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
}

void ComponentManager::Cleanup()
{
}
