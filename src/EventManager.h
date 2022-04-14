#pragma once
#include <queue>
#include "Event.h"

using namespace std;
class EventManager
{
public:
	void Enqueue(Event);
	void ResolveEvents();

private:
	queue<Event> eventQueue;
};