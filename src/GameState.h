#pragma once
#include <iostream>
#include <iomanip>
#include "RandomGenerator.h"
class ComponentManager;
using namespace std;



class GameState {
public:
    GameState(const int init, ComponentManager* man) :
        compMan(man),
        INITIAL_OBJECT_COUNT(init),
        currentObjectCount(init),
        objectsCollected(0),
        SECONDS_BETWEEN_NEW_GAME_OBJ(5),
        spawnStarFrames(0),
        spawnSandFrames(0),
        cumulativeFrameTime(0),
        TotalObjectsEverMade(init),
        randSand(make_unique<RandomGenerator>(1,3))
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
        cout << "\nTotal Objects Remaining: " << objectsNeeded;
        cout << "\n---------------------------------" << endl;
    }
    void IncTotalFrameTime(float frameTime) {
        cumulativeFrameTime += frameTime; 
        spawnStarFrames += frameTime;
        spawnSandFrames += frameTime;
    }
    void EndGame()
    {
        cout << "You have collected: " << objectsCollected << " out of " << objectsNeeded << " star fragments in "
            << cumulativeFrameTime << " seconds, a rate of " << setprecision(2) <<
            (objectsCollected / cumulativeFrameTime) << " per second." << endl;
        if (objectsCollected >= objectsNeeded)
            cout << "YOU WIN! Collected enough star fragments!" << endl;
        else
            cout << "YOU LOST! Did not collect enough star fragments, you are stranded." << endl;
        isGameEnded = true;
    }
    float GetTotalFrameTime() const { return cumulativeFrameTime; }
    const int ReportObjectsCollected() const { return objectsCollected; }
    //spawnFrames is the number of seconds since an object was last spawned.
    bool ShouldSpawnStar() {
        if (spawnStarFrames >= SECONDS_BETWEEN_NEW_GAME_OBJ) {
            spawnStarFrames = 0;
            currentObjectCount++;
            return true;
        }
        return false;
    }
    bool IsGameEnded() { return isGameEnded; }

    //for now just do it every time you spawn star fragments.
    bool ShouldSpawnSand() {
        if (spawnSandFrames >= 4) { 
            spawnSandFrames = 0;
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
    float spawnStarFrames;
    float spawnSandFrames;
    float cumulativeFrameTime;
    bool isGameEnded = false;
    int objectsNeeded = 50;
    std::unique_ptr<RandomGenerator> randSand;
};
