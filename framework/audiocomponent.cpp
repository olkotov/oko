// Oleg Kotov

#include "audiocomponent.h"
#include "sound.h"
#include "okoengine.h"
#include "resourcemanager.h"

#include <AL/al.h>

AudioComponent::AudioComponent()
{
	alGenSources( 1, &m_sourceHandle );

	Vector origin = getWorldPosition();

	float position[3]; // ? alSource3f( audioSource, AL_POSITION, 0.0f, 0.0f, 0.0f );
	position[0] =  origin.x;
	position[1] =  origin.z;
	position[2] = -origin.y;

	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;

	// alSourcefv( _sourceHandle, AL_POSITION, position );
	// alSourcefv( _sourceHandle, AL_VELOCITY, velocity );

	// alSourcef( _sourceHandle, AL_REFERENCE_DISTANCE, 0.0f );

	// alSourcef( _sourceHandle, AL_PITCH, 1.0f ); // frequency (or speed)
	// alSourcef( _sourceHandle, AL_GAIN,  1.0f ); // volume
	alSourcei( m_sourceHandle, AL_LOOPING, false );
	
	// alSourcei( _sourceHandle, AL_CONE_INNER_ANGLE, 90 );
	// alSourcei( _sourceHandle, AL_CONE_OUTER_ANGLE, 120 );
}

AudioComponent::~AudioComponent()
{
	alDeleteSources( 1, &m_sourceHandle );
}

void AudioComponent::setSound( Sound* sound )
{
	if ( !sound ) return;

	stop();

	m_sound = sound;
	alSourcei( m_sourceHandle, AL_BUFFER, m_sound->getSoundId() );
}

void AudioComponent::play() const
{
	alSourcePlay( m_sourceHandle );
}

void AudioComponent::play( const std::string& filename )
{
	Sound* sound = engine()->resources()->getSound( filename );
	assert( sound != nullptr );

	setSound( sound );
	play();
}

void AudioComponent::pause() const
{
	alSourcePause( m_sourceHandle );
}

void AudioComponent::stop() const
{
	alSourceStop( m_sourceHandle );
}

bool AudioComponent::isPlaying() const
{
	int state = 0;
	alGetSourcei( m_sourceHandle, AL_SOURCE_STATE, &state );

	return ( state == AL_PLAYING );
}

void AudioComponent::setLoop( bool loop ) const
{
	alSourcei( m_sourceHandle, AL_LOOPING, loop );
}

