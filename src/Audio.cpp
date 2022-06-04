#include "Audio.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
using namespace std;

#define DEBUG_MA
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

void AudioEngine::InitSoundFromFile(string filename, string id)
{
    sounds[filename + id] = make_unique<ma_sound>();
    CHECK_MA(ma_sound_init_from_file(engine.get(), (resourceDir + filename).c_str(), 0, NULL, NULL, sounds[filename + id].get()));
}

void AudioEngine::Init(string resourceDir)
{
    this->resourceDir = resourceDir + "/";
    engine = make_unique<ma_engine>();
    CHECK_MA(ma_engine_init(NULL, engine.get()));

    InitSoundFromFile("tomorrow.mp3");
    for (int i = 0; i < 100; i++) {
        InitSoundFromFile("collect_majortriad.mp3", to_string(i));
    }
    
}

void AudioEngine::Play(string soundFileName)
{
    // commented out music for sanity purposes
    ma_result result = ma_sound_start(sounds[soundFileName].get());
    check(result);
}

void AudioEngine::PlayClocked(string soundFileName) {
    //engine_get_time returns time in PCM frames. convert to seconds.
    float currTime = ma_engine_get_time(engine.get()) / static_cast<float>(ma_engine_get_sample_rate(engine.get()));
    if (lastTime == 0 || currTime - lastTime > pitchAdjustPeriod) {
        halfSteps = 0; //more than the pitchAdjustPeriod has passed, start from the base pitch.
    } 
    int index = 1 + (soundsPlayed % (sounds.size() - 2));

    ma_sound_set_pitch(sounds[soundFileName + to_string(index)].get(), static_cast<float>(pow(2, (halfSteps) / 12.0f)));
    ma_result result = ma_sound_start(sounds[soundFileName + to_string(index)].get());
    halfSteps++; //the next one is played a half-step higher.
    soundsPlayed++;
    lastTime = currTime;
}

void AudioEngine::Cleanup()
{
    ma_engine_uninit(engine.get());
}
