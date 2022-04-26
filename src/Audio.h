#pragma once
#include <memory>
#include <iostream>
#include <unordered_map>
#include "miniaudio.h"

class AudioEngine {
public:
    void Init(std::string resourceDir);
    void InitSoundFromFile(std::string soundFilename);
    void Play(std::string soundFileName);
    void Cleanup();
private:
    void check(ma_result result);
    
    std::unique_ptr<ma_engine> engine;
    std::unordered_map<std::string, std::unique_ptr<ma_sound>> sounds;
    std::string resourceDir;
};