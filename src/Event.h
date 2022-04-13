#pragma once
#include <utility>

using namespace std;
class Event
{
public:
  void Subscribe(Component);
  void Unsubscribe(Component);
private:
  vector<pair<Component, <U, Ts...>> listeners;

}