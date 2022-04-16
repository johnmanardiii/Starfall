#include "GameState.h"
#include "ComponentManager.h"
int GameState::GetCount()
{
    return compMan->CurrentObjectCount();
}