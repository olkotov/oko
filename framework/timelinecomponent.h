// Oleg Kotov

#pragma once

#include "entitycomponent.h"
#include "event.h"

#include <functional>

using UpdateEvent = Event<float>;
using FinishedEvent = Event<>;

// using TimelineCallback = std::function<void( float value )>;

class ENGINE_API TimelineComponent : public EntityComponent
{
	typedef EntityComponent Super;

public:

	void tick( float deltaTime ) override;

	void play();
	void playFromStart();
	void reverse();
	void reverseFromEnd();
	void stop();

	void setDuration( float duration );

	// void bind( TimelineCallback callback );

private:

	bool m_isPlaying = false;
	bool m_isReverse = false;

	float m_elapsedTime = 0.0f;
	float m_duration = 1.0f;

	float m_currentValue = 0.0f;

	// TimelineCallback m_callback;

public:

	UpdateEvent updateEvent;
	FinishedEvent finishedEvent;
};

