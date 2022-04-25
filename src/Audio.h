#pragma once
#include <memory>
#include <iostream>

#include "miniaudio.h"

class AudioEngine {
public:
    void Init(std::string resourceDir);
    void PlaySoundOnce(std::string soundFileName);
    void Cleanup();
private:
    void check(ma_result result);
    std::unique_ptr<ma_engine> engine;
    std::string resourceDir;
};