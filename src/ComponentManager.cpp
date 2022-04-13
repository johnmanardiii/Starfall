#include "ComponentManager.h"

void ComponentManager::deleteObject(GameObject obj)
{
    // TODO removes all components specified in a Game Object's object list. 
    // e.x. gameobject with (0, -1, 1) removes first component, first position, and third component, second position.
    // just realized that references will change if positions are shifted around.
}

GameObject ComponentManager::getObject(string name)
{
    //TODO this should return a reference/copy (discuss) to a game object
    //based on the key in a map.
    return GameObject();
}

void ComponentManager::init()
{
    
    camera = Camera::getInstance();
}

void ComponentManager::cleanup()
{
}
