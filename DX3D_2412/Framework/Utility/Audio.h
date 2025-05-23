#pragma once

using namespace FMOD;

class Audio : public Singleton<Audio>
{
private:
    friend class Singleton;

    const int MAX_CHANNEL = 30;

    struct SoundInfo
    {
        Sound* sound = nullptr;
        Channel* channel = nullptr;

        ~SoundInfo()
        {
            sound->release();
        }
    };

    Audio();
    ~Audio();

public:
    void Update();

    void Add(string key, string file,
        bool bgm = false, bool loop = false, bool is3D = false);

    void Play(string key, float volume = 1.0f);
    void Play(string key, Float3 position, float volume = 1.0f);
    void Stop(string key);
    void Pause(string key);
    void Resume(string key);
    void LoadAudioFiles(const string& path);

    bool IsPlaySound(string key);

private:
    System* soundSystem;

    unordered_map<string, SoundInfo*> sounds;

    FMOD_VECTOR listenerPos;
};