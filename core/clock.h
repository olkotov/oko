// Oleg Kotov

#pragma once

#include "baseapi.h"
#include <chrono>

/**
* Represents a game clock.
***********************************************************************************/

class ENGINE_API Clock
{
public:

	Clock();

	// Gets the elapsed time since the start of the game, in seconds.
	float getTimeSinceStartup() const;

	// Calculate the delta time.
	float update();

	// Gets the elapsed time between the start of the previous and current frames, in seconds.
	float getDeltaTime() const;

	//
	float getUnscaledDeltaTime() const;

	//
	void setTimeScale( float timeScale );

	//
	float getTimeScale() const;

	// Returns the number of seconds for this date since the epoch.
	uint64_t getUnixTime() const;

private:

	//
	float m_timeScale = 1.0f;

	// The previous frame time.
	float m_previousTime = 0.0f;

	// The delta time between previous and current frames.
	float m_deltaTime = 0.0f;

	//
	std::chrono::steady_clock::time_point m_startTime;
};

