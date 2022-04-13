#pragma once
//"interface" (no interface in c++) to reinforce that components should have an update
class Component
{
public:
    //Components must have an update function, even if it does nothing
    virtual void update() = 0;
private:
};

