// Oleg Kotov

#include "sound.h"
#include <AL/al.h>

Sound::Sound()
{
    alGenBuffers( 1, &m_bufferHandle );
}

Sound::~Sound()
{
    alDeleteBuffers( 1, &m_bufferHandle );
}

