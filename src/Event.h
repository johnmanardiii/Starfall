#pragma once
#include <utility>
#include <vector>

#include "Component.h"

using namespace std;
class Event
{
	typedef pair<Component*, void*> Listener;

public:
	void Subscribe(Listener);
	void Unsubscribe(Listener);

	virtual void Send();
private:
	vector<Listener> listeners;
};