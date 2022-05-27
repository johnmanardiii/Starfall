#pragma once
#include <iostream>
#include <iomanip>
#include "RandomGenerator.h"
class ComponentManager;
using namespace std;
using namespace glm;


class GameState {
public:
    GameState(const int init, ComponentManager* man) :
        compMan(man),
        INITIAL_OBJECT_COUNT(init),
        currentObjectCount(init),
        objectsCollected(0),
        distanceTravelled(vec3(0)),
        spawnStarFrames(0),
        spawnSandFrames(0),
        cumulativeFrameTime(0),
        TotalObjectsEverMade(init)
    {}
    const int GetInitialCount() const { return INITIAL_OBJECT_COUNT; }
    int GetCount() { return
        currentObjectCount;
    }
    void Collect();
    void IncTotalFrameTime(float frameTime);
    void EndGame();
    float GetTotalFrameTime() const { return cumulativeFrameTime; }
    const int ReportObjectsCollected() const { return objectsCollected; }
    bool IsGameEnded() { return isGameEnded; }
    bool ShouldSpawnSand();
    bool ShouldSpawnStar();

    int TotalObjectsEverMade;
private:
    ComponentManager* compMan;
    const int INITIAL_OBJECT_COUNT;
    int currentObjectCount;
    int objectsCollected;
    vec3 prevPlayerPos;
    vec3 distanceTravelled;
    const float SPAWN_STAR_DIST = 60;
    float spawnStarFrames;
    float spawnSandFrames;
    float cumulativeFrameTime;
    bool isGameEnded = false;
    int objectsNeeded = 50;
};
