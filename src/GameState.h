#pragma once
#include <iostream>

class ComponentManager;
using namespace std;
class GameState {
public:
    GameState(const int init, ComponentManager* man) :
        compMan(man),
        INITIAL_OBJECT_COUNT(init),
        currentObjectCount(INITIAL_OBJECT_COUNT),
        objectsCollected(0),
        secondsBetweenNewObject(5),
        cumulativeFrameTime(0)
    {}
    const int GetInitialCount() const { return INITIAL_OBJECT_COUNT; }
    int GetCount() { return
        (static_cast<int>(cumulativeFrameTime) / secondsBetweenNewObject) + currentObjectCount; //purposefully do integer divide.
    }
    void Collect() {
        objectsCollected++;
        currentObjectCount--;
        cout << "\n---------------------------------";
        cout << "\nObject Collected!";
        cout << "\nTotal Objects Collected:" << objectsCollected;
        cout << "\nTotal Objects Remaining: " << GetCount();
        cout << "\n---------------------------------";
    }
    void IncTotalFrameTime(float frameTime) { cumulativeFrameTime += frameTime; }
    float GetTotalFrameTime() const { return cumulativeFrameTime; }
    const int ReportObjectsCollected() const { return objectsCollected; }

private:
    ComponentManager* compMan;
    const int INITIAL_OBJECT_COUNT;
    int currentObjectCount;
    int objectsCollected;
    int secondsBetweenNewObject;
    float cumulativeFrameTime;
};