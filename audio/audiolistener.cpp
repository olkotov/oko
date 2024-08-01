// Oleg Kotov

#include "audiolistener.h"

#include <AL/al.h>

void AudioListener::tick( float deltaTime )
{
	alListenerf( AL_GAIN, 1.0f );

	Vector origin = getWorldPosition();
	Vector viewDirection = getForwardDirection();
	Vector upDirection = getUpDirection();
	
	float position[3];
	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;

	// alListener3f( AL_POSITION, position[0], position[1], position[2] ); // alListenerfv

	ALfloat listenerOrientation[6];

	listenerOrientation[0] =  viewDirection.x;
	listenerOrientation[1] =  viewDirection.z;
	listenerOrientation[2] = -viewDirection.y;

	listenerOrientation[3] =  upDirection.x;
	listenerOrientation[4] =  upDirection.z;
	listenerOrientation[5] = -upDirection.y;

	listenerOrientation[0] =  0.0f;
	listenerOrientation[1] =  0.0f;
	listenerOrientation[2] = -1.0f;
	listenerOrientation[3] =  0.0f;
	listenerOrientation[4] =  1.0f;
	listenerOrientation[5] =  0.0f;

	// alListenerfv( AL_ORIENTATION, listenerOrientation );
}

