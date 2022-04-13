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
    return GameObject();
}

void ComponentManager::Init()
{
    
    camera = Camera::GetInstance();
}

void ComponentManager::UpdateComponents()
{
    camera.Update();
}

void ComponentManager::Cleanup()
{
}
