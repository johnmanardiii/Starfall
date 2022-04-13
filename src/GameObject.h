#pragma once
#include <map>
#include <typeinfo>
using std::map;

class GameObject
{
public:
    
private:
    map<type_info*, int> components; //component or int to point to component
};

