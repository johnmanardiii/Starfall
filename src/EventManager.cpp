#include "EventManager.h"

void EventManager::Enqueue(Event newEvent)
{
  eventQueue.push(newEvent);
}

void EventManager::ResolveEvents()
{
  /*
  while (!eventQueue.empty())
  {
    eventQueue.front().Send();
    eventQueue.pop();
  }
  */
}