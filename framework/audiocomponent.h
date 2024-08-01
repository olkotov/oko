// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "scenecomponent.h"

#include <stdint.h>
#include <string>

class Sound;

class ENGINE_API AudioComponent : public SceneComponent
{
public:

	AudioComponent();
	~AudioComponent();

	// TODO: tick()

	void setSound( Sound* sound );
	void play() const;
	void pause() const;
	void stop() const;
	bool isPlaying() const;
	void setLoop( bool loop ) const;

	void play( const std::string& filename );

public:

	bool autoActivate = false;

private:

	Sound* m_sound = nullptr;
	uint32_t m_sourceHandle = 0;
};

