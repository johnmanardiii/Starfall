#pragma once

//forward declare
class ComponentManager;
class Movement;
class Transform;
#include <string>

//"interface" (no interface in c++) to reinforce that components should have an update
class Component
{
public:
    Component() : Name("unitialized") {}
    Component(std::string gameObjectName) { Name = gameObjectName; }
    //Components must have an update function, even if it does nothing
    virtual void Update() = 0;
    virtual void Init(ComponentManager&) = 0;
    bool IsActive = true;
    std::string Name = "unitialized";
private:
};

