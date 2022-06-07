#include "GameState.h"
#include "ComponentManager.h"
#include "Player.h"
bool GameState::ShouldSpawnStar() {
    //retrieve the 
    const vec3& currPlayerPos = Player::GetInstance(vec3(0)).GetPosition();
    distanceTravelled += currPlayerPos - prevPlayerPos;
    prevPlayerPos = currPlayerPos;
    if (length2(distanceTravelled) > pow(SPAWN_STAR_DIST, 2)) {
        distanceTravelled = vec3(0);
        currentObjectCount++;
        return true;
    }
    else {
        return false;
    }
}

void GameState::Collect() {
    objectsCollected++;
    currentObjectCount--;
    cout << "\n---------------------------------";
    cout << "\nObject Collected!";
    cout << "\nTotal Objects Collected:" << objectsCollected;
    cout << "\nTotal Objects Remaining: " << objectsNeeded;
    cout << "\n---------------------------------" << endl;
}

void GameState::IncTotalFrameTime(float frameTime) {
    cumulativeFrameTime += frameTime;
    spawnStarFrames += frameTime;
    spawnSandFrames += frameTime;
}

void GameState::EndGame()
{
    cout << "You have collected: " << objectsCollected << " out of " << objectsNeeded << " star fragments in "
        << cumulativeFrameTime << " seconds, a rate of " << setprecision(2) <<
        (objectsCollected / cumulativeFrameTime) << " per second." << endl;
    if (objectsCollected >= objectsNeeded) {
        cout << "YOU WIN! Collected enough star fragments!" << endl;
        wonGame = true;
    }
    else
        cout << "YOU LOST! Did not collect enough star fragments, you are stranded." << endl;
    isGameEnded = true;
}
//when or what conditions to spawn sand particles
bool GameState::ShouldSpawnSand() {
    auto& player = Player::GetInstance(vec3());
    if (player.GetCurrentSpeedAsPct() > 0.7) {
        spawnSandFrames = 0;
        currentObjectCount++;
        return true;
    }
    return false;
}

void GameState::InitLights() {
    for (int i = 0; i < 20; i++) {
        lights.push_back(0);
        lights.push_back(FLT_MAX);
        lights.push_back(FLT_MAX);
    }
}
//warning, does nothing atm
void GameState::UpdateLights() {
    return;
}