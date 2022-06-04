#pragma once
#include <memory>
#include <iostream>
#include <unordered_map>
#include "miniaudio.h"
#include <string>

using std::string;

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
    int soundsPlaying;
    
    
    std::unique_ptr<ma_engine> engine;
    std::unordered_map<string, std::unique_ptr<ma_sound>> sounds;
    string resourceDir;
};