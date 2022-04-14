#pragma once
#include <utility>
#include <vector>
#include <functional>

#include "Component.h"
#include "ComponentManager.h"
using namespace std;

// pair: 
//   pointer to listening componend
//   pointer to callback method
typedef pair<Component*, void*> Listener;

class Event
{
public:
	virtual void Subscribe(Listener);
	virtual void Unsubscribe(Listener);

  // User needs to define send
};

// class CollisionEvent : Event
// {
//   typedef pair<Component*, std::function<void(int, int)>> CollisionListener;
// public:
//   void Subscribe(CollisionListener);
//   void Unsubscribe(CollisionListener);

//   void Send(int sender, int other) 
//   {
//     for (std::vector<CollisionListener>::iterator it = listeners.begin(); it != listeners.end(); ++it)
//    	{
// 		  Component* comp = it->first;
// 			auto callback = it->second;

//       if(auto ptr = dynamic_cast<Movement*>(comp)){ //refers to movement component

//       }
//       else if (auto ptr = dynamic_cast<Transform*>(comp)){ //transform component
//         if(auto f = dynamic_cast<void(Transform::*)(int, int)>(callback)){
          
//         }
//       }
//       comp->callback(sender, other);
// 		}
//   }

// private:
//   vector<CollisionListener> listeners;
  
// };