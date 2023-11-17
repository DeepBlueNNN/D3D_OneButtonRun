#include "Framework.h"

Audio::Audio()
{
    System_Create(&soundSystem);
    soundSystem->init(MAX_CHANNEL, FMOD_INIT_NORMAL, nullptr);
}

Audio::~Audio()
{
    for (pair<string, SoundInfo*> sound : sounds)
        delete sound.second;

    soundSystem->release();
}

void Audio::Update()
{
    soundSystem->update();
}

void Audio::Add(string key, string file, bool bgm, bool loop)
{
    if (sounds.count(key) > 0) return;

    SoundInfo* info = new SoundInfo();

    if (bgm)
    {
        soundSystem->createStream(file.c_str(),
            FMOD_LOOP_NORMAL, nullptr, &info->sound);
    }
    else
    {
        if (loop)
        {
            soundSystem->createSound(file.c_str(),
                FMOD_LOOP_NORMAL, nullptr, &info->sound);
        }
        else
        {
            soundSystem->createSound(file.c_str(),
                FMOD_DEFAULT, nullptr, &info->sound);
        }
    }

    sounds[key] = info;
}

void Audio::Play(string key, float valume)
{
    if (sounds.count(key) == 0) return;

    soundSystem->playSound(sounds[key]->sound,
        nullptr, false, &sounds[key]->channel);
    sounds[key]->channel->setVolume(valume);
}

void Audio::Stop(string key)
{
    if (sounds.count(key) == 0) return;

    sounds[key]->channel->stop();
}

void Audio::Pause(string key)
{
    if (sounds.count(key) == 0) return;

    sounds[key]->channel->setPaused(true);
}

void Audio::Resume(string key)
{
    if (sounds.count(key) == 0) return;

    sounds[key]->channel->setPaused(false);
}

bool Audio::IsPlaySound(string key)
{
    if (sounds.count(key) == 0) return false;

    bool isPlay = false;
    sounds[key]->channel->isPlaying(&isPlay);

    return isPlay;
}
