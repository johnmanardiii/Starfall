#include "Audio.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
using namespace std;

void AudioEngine::check(ma_result result)
{
    if (result != MA_SUCCESS) {
        cerr << "miniaudio call did not return successful state." << endl;
        throw std::runtime_error("miniaudio");
    }
}

void AudioEngine::Init(string resourceDir)
{
    this->resourceDir = resourceDir + "/";
    engine = make_unique<ma_engine>();
    ma_result result = ma_engine_init(NULL, engine.get());
    check(result);
}

void AudioEngine::PlaySoundOnce(string soundFileName)
{
    ma_result result = ma_engine_play_sound(engine.get(), (resourceDir + soundFileName).c_str(), NULL);
    check(result);
}

void AudioEngine::Cleanup()
{
    ma_engine_uninit(engine.get());
}
