#include "Audio.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
using namespace std;

//#define DEBUG_MA
#define CHECK_MA(result) result
#ifdef DEBUG_MA
#define CHECK_MA(result) check(result)
#endif


void AudioEngine::check(ma_result result)
{
    if (result != MA_SUCCESS) {
        cerr << "miniaudio call did not return successful state." << endl;
        throw std::runtime_error("miniaudio");
    }
}

void AudioEngine::InitSoundFromFile(string filename)
{
    sounds[filename] = make_unique<ma_sound>();
    CHECK_MA(ma_sound_init_from_file(engine.get(), (resourceDir + filename).c_str(), 0, NULL, NULL, sounds[filename].get()));
}

void AudioEngine::Init(string resourceDir)
{
    this->resourceDir = resourceDir + "/";
    engine = make_unique<ma_engine>();
    CHECK_MA(ma_engine_init(NULL, engine.get()));

    InitSoundFromFile("tomorrow.mp3");
}

void AudioEngine::Play(string soundFileName)
{
    // commented out music for sanity purposes
    //ma_result result = ma_sound_start(sounds[soundFileName].get());
    //check(result);
}

void AudioEngine::Cleanup()
{
    ma_engine_uninit(engine.get());
}
