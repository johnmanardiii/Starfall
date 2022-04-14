#include "Event.h"
#include <algorithm>


void Event::Subscribe(Listener key)
{
	listeners.push_back(key);
}

void Event::Unsubscribe(Listener key)
{
	std::vector<Listener>::iterator position = std::find(listeners.begin(), listeners.end(), key);
	if (position != listeners.end()) // == myVector.end() means the element was not found
		listeners.erase(position);
}