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
    if (it != objects.end())
        return it->second;
    std::cerr << "GetObject did not"
}

void ComponentManager::Init()
{
    
    camera = Camera::GetInstance();
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
    
    for (const auto& comp : comps) {
        if (!comp) {
            typeid(Component);
        }
    }
    
}

void ComponentManager::RemoveGameObject(string name)
{
}

void ComponentManager::Cleanup()
{
}
