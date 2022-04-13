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
    //declaration style 2, with no movement component.
    auto StaticBunny = vector<shared_ptr<Component>>{ renderer, movement, transform, collision };
    AddGameObject("staticBunny", StaticBunny);

}

void ComponentManager::UpdateComponents()
{

    camera.Update();
}

GameObject ComponentManager::AddGameObject(string name, vector<shared_ptr<Component>> comps)
{
    map<type_info*, size_t> componentList;
    //don't care what container is used to pass in components,
    //Pad unused components with null.
    map<string, size_t> objComps;
    for (const auto& comp : comps) {
        if (comp) { //null check
            addToCollection(comp);
        }
    }
    return GameObject();
}

void ComponentManager::addToCollection(const shared_ptr<Component>& comp) {
    auto ptr = dynamic_pointer_cast<Transform>(comp);
    cout << typeid(ptr).name() << endl;
}

void ComponentManager::RemoveGameObject(string name)
{
}

void ComponentManager::Cleanup()
{
}
