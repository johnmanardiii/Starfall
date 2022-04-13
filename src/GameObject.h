#pragma once
#include <map>
#include <typeinfo>
#include <string>

using std::map;
using std::string;

class GameObject
{
public:
    bool IsActive;
    string Name;
private:
    map<string, int> components; //component or int to point to component
};

