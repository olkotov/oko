// Oleg Kotov

#include "soundsystem.h"
#include "log.h"

SoundSystem::SoundSystem()
{
	Msg( "soundsystem:initialize" );

	// retrieve the default device name
	const char* defaultDeviceName = alcGetString( NULL, ALC_DEFAULT_DEVICE_SPECIFIER );

	// open the default device

	m_audioDevice = alcOpenDevice( defaultDeviceName );
	// m_audioDevice = alcOpenDevice( nullptr );

	if ( m_audioDevice == nullptr )
	{
		Warning( "unable to open default device" );
	}

	// create context

	m_audioContext = alcCreateContext( m_audioDevice, nullptr );

	if ( m_audioContext == nullptr )
	{
		Warning( "failed to create context" );
	}

	// set active context

	if ( !alcMakeContextCurrent( m_audioContext ) )
    {
		Warning( "failed to make default context" );
    }
}

SoundSystem::~SoundSystem()
{
	Msg( "soundsystem:shutdown" );

	alcMakeContextCurrent( nullptr );
	alcDestroyContext( m_audioContext );
	alcCloseDevice( m_audioDevice );
}

//void SoundSystem::play( Sound* sound )
//{
//
//}

//void SoundSystem::update( float deltaTime )
//{
//
//}

