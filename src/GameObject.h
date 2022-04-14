#pragma once
#include <map>
#include <typeinfo>
#include <string>

using std::map;
using std::string;

class GameObject
{
public:
    GameObject() :
        Name("test"),
        IsActive(true),
        components() {};
    GameObject(string name, map<string, size_t> comps) : 
        Name(name),  
        IsActive(true),
        components(comps){};
    bool IsActive; //whether the component is active. might remove.
    string Name; //the name of the object.
    const map<string, size_t>& GetComponentLocations() { return components; }
    int GetComponentLocation(string comp) {return components[comp];}
private:
    map<string, size_t> components; //component or int to point to component
};

