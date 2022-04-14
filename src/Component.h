#pragma once

//forward declare
class ComponentManager;
class Movement;
class Transform;
class Collect;
class TextureRenderer;
#include <string>

//"interface" (no interface in c++) to reinforce that components should have an update
class Component
{
public:
    //delete copy constructor
    //Component(const Component&) = delete;
    //Component& operator=(const Component&) = delete;

    Component(std::string gameObjectName) : Name(gameObjectName) {}
    //Components must have an update function, even if it does nothing
    virtual void Update(float frameTime) = 0;
    virtual void Init(ComponentManager&) = 0;
    bool IsActive = true;
    std::string Name = "unitialized";
private:
};

