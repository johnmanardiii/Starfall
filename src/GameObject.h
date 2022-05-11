#pragma once
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <string>

using std::unordered_map;
using std::vector;
using std::string;


class GameObject
{
public:
    GameObject() :
        Name("test"),
        IsActive(true),
        components() {};
    GameObject(string name, unordered_map<string, size_t> comps) :
        Name(name),  
        IsActive(true),
        components(comps) {};
    bool IsActive; //whether the component is active. might remove.
    string Name; //the name of the object.
    const unordered_map<string, size_t>& GetComponentLocations() { return components; }
    size_t GetComponentLocation(string comp) {return components[comp];}
private:
    unordered_map<string, size_t> components; //component or int to point to component
};

