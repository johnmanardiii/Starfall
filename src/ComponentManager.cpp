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
    //TODO change the right-hand pointer to a derived component.
    shared_ptr<Component> renderer = shared_ptr<Component>();
    shared_ptr<Component> movement = shared_ptr<Component>();
    shared_ptr<Component> transform = shared_ptr<Component>();
    shared_ptr<Component> collision = std::make_shared<>();
    vector<shared_ptr<Component>> vBunny = { renderer, movement, transform, collision };
    vector<shared_ptr<Component>> vInvis = { nullptr, movement, transform, collision };
    AddGameObject("bunny", vBunny);
    AddGameObject("invisibleBunny", vInvis);
    AddGameObject("staticBunny", vector<shared_ptr<Component>>{ renderer, nullptr, transform, collision });

}

void ComponentManager::UpdateComponents()
{

    camera.Update();
}

GameObject ComponentManager::AddGameObject(string name, vector<shared_ptr<Component>> comps)
{
    map<type_info*, size_t> componentList;
    //don't care what container is used to pass in components, 
    //just as long as it is ordered and has same size as max components.
    //Pad unused components with null.
    map<string, size_t> objComps;
    for (const auto& comp : comps) {
        if (comp) {
            std::cout << typeid(comp).name() << std::endl;
        }
    }
    return GameObject();
}
   

void ComponentManager::RemoveGameObject(string name)
{
}

void ComponentManager::Cleanup()
{
}
