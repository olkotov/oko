// Oleg Kotov

#pragma once

#include "okobase.h"

#include <AL/al.h>
#include <AL/alc.h>

class Sound;

class ENGINE_API SoundSystem
{
public:

    SoundSystem();
    ~SoundSystem();

    // void play( Sound* sound );
    // void update( float deltaTime );

private:

    ALCdevice* m_audioDevice = nullptr;
    ALCcontext* m_audioContext = nullptr;
};

