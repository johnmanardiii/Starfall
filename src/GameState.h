#pragma once
class ComponentManager;
class GameState {
public:
    GameState(const int init, ComponentManager* man) :
        compMan(man),
        INITIAL_OBJECT_COUNT(init),
        objectsCollected(0)
    {}
    const int GetInitialCount() const { return INITIAL_OBJECT_COUNT; }
    int GetCount();
    void Collect() { objectsCollected++; }

private:
    ComponentManager* compMan;
    const int INITIAL_OBJECT_COUNT;
    int objectsCollected;
};