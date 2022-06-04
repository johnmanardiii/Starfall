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

    InitSoundFromFile("tomorrow.mp3", to_string(0));
    for (int i = 1; i < 21; i++) {
        InitSoundFromFile("collect_majortriad.mp3", to_string(i));
    }
    
}

void AudioEngine::Play(string soundFileName)
{
    // commented out music for sanity purposes
    ma_result result = ma_sound_start(sounds[soundFileName + to_string(0)].get());
    check(result);
}

void AudioEngine::PlayClocked(string soundFileName) {
    soundsPlaying = 0;
    for (pair<const string, unique_ptr<ma_sound>>& sound : sounds){
        if (ma_sound_is_playing(sound.second.get())) {
            soundsPlaying++;
        }
    }
    if (soundsPlaying == sounds.size()) {
        //ma_sound_stop(sounds[soundIndex].get());
        soundsPlaying--; //just play the last one again
    }
    string soundIndex = soundFileName + to_string(soundsPlaying);
    ma_sound_set_pitch(sounds[soundIndex].get(), static_cast<float>(pow(2,(soundsPlaying-1)/12.0f)));
    if (!ma_sound_is_playing(sounds[soundIndex].get())) {
        ma_result result = ma_sound_start(sounds[soundIndex].get());
        check(result);
    }
}

void AudioEngine::Cleanup()
{
    ma_engine_uninit(engine.get());
}
