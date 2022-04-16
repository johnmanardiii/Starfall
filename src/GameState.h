#pragma once
#include <iostream>
#include <iomanip>

class ComponentManager;
using namespace std;
class GameState {
public:
    GameState(const int init, ComponentManager* man) :
        compMan(man),
        INITIAL_OBJECT_COUNT(init),
        currentObjectCount(init),
        objectsCollected(0),
        SECONDS_BETWEEN_NEW_GAME_OBJ(10),
        spawnFrames(0),
        cumulativeFrameTime(0),
        TotalObjectsEverMade(init)
    {}
    const int GetInitialCount() const { return INITIAL_OBJECT_COUNT; }
    int GetCount() { return
        currentObjectCount;
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
    void IncTotalFrameTime(float frameTime) {
        cumulativeFrameTime += frameTime; 
        spawnFrames += frameTime;
    }
    float GetTotalFrameTime() const { return cumulativeFrameTime; }
    const int ReportObjectsCollected() const { return objectsCollected; }
    //spawnFrames is the number of seconds since an object was last spawned.
    bool ShouldSpawn() {
        if(currentObjectCount == 0){
            cout << "You Win! Collected: " << objectsCollected << " objects in "
                << cumulativeFrameTime << " seconds, a rate of " << setprecision(2) <<
            (objectsCollected / cumulativeFrameTime) << " per second." << endl;
            setprecision(defaultp);
            exit(EXIT_SUCCESS);
        }
        if (spawnFrames >= SECONDS_BETWEEN_NEW_GAME_OBJ) {
            spawnFrames = 0;
            currentObjectCount++;
            return true;
        }
        return false;
    }


    int TotalObjectsEverMade;

private:
    ComponentManager* compMan;
    const int INITIAL_OBJECT_COUNT;
    int currentObjectCount;
    int objectsCollected;
    const int SECONDS_BETWEEN_NEW_GAME_OBJ;
    float spawnFrames;
    float cumulativeFrameTime;
    
};
