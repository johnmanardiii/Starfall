#pragma once
class ComponentManager;
class GameState {
public:
    GameState(const int init, ComponentManager* man) :
        compMan(man),
        INITIAL_OBJECT_COUNT(init),
        objectsCollected(0),
        cumulativeFrameTime(0)
    {}
    const int GetInitialCount() const { return INITIAL_OBJECT_COUNT; }
    int GetCount();
    void Collect() { objectsCollected++; }
    void IncTotalFrameTime(float frameTime) { cumulativeFrameTime += frameTime; }
private:
    ComponentManager* compMan;
    const int INITIAL_OBJECT_COUNT;
    int objectsCollected;
    float cumulativeFrameTime;
};