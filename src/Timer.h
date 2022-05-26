#ifndef TIMER_H
#define TIMER_H
#include <iostream>
#include <chrono>
#include <string>
using namespace std::chrono;
class Timer {
    
    time_point<high_resolution_clock> startTime;
    time_point<high_resolution_clock> endTime;
    
    
public:
    void start();
    int stop();
    void pStop(std::string event);
};
#endif
