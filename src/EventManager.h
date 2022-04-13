#pragma once
#include <queue>

using namespace std;
class EventManager
{
public:
  void Enqueue(Event);
  void ResolveEvents();
private:
  queue eventQueue;
}