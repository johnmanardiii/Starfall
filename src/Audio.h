#pragma once
#include <memory>
#include <iostream>
#include <unordered_map>
#include "miniaudio.h"
#include <string>

using std::string;
using std::vector;

class AudioEngine {
public:
    void Init(string resourceDir);
    void InitSoundFromFile(string filename);
    void InitSoundFromFile(string soundFilename, string id);
    void Play(string soundFileName);
    void PlayClocked(string soundFileName);
    void Cleanup();
private:
    void check(ma_result result);
    const float pitchAdjustPeriod = 1.0f; //if a star fragment is collected less than this time in seconds since the last one, adjust the pitch.
    float lastTime = 0.0f; //the actual time in seconds since a pitch-adjusted sound was played.
    int halfSteps = 0; //the number of half steps to adjust the pitch by.
    int soundsPlayed = 0; //a number to use w/ modulus for multiple of the same sound.
    std::unique_ptr<ma_engine> engine;
    std::unordered_map<string, std::unique_ptr<ma_sound>> sounds;
    string resourceDir;
    vector<int> minorScale = { 0,2,3,5,7,8,10,12 };
};