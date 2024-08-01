// Oleg Kotov

#pragma once

#include "baseapi.h"

#include <functional>
#include <unordered_map>
#include <assert.h>

using TimerCallback = std::function<void()>;
using TimerHandle = size_t;

class ENGINE_API TimerManager
{
public:

	struct Timer
	{
		bool active = false;

		float elapsedTime = 0.0f;
		float duration = 0.0f;

		float rate = 0.0f;
		bool loop = false;
		float firstDelay = 0.0f;

		TimerCallback callback = nullptr;
	};

	~TimerManager()
	{
		clearTimers();
	}

	void clearTimers()
	{
		timers.clear();
	}

	TimerHandle createTimer()
	{
		TimerHandle handle = nextTimerHandle++;
		timers.emplace( handle, Timer() );
		return handle;
	}

	void destroyTimer( TimerHandle handle )
	{
		assert( isTimerExists( handle ) && "Timer does not exist." );
		timers.erase( handle );
	}

	bool setTimer( TimerHandle handle, TimerCallback callback, float rate, bool loop = false, float firstDelay = 0.0f )
	{
		auto timerIt = timers.find( handle );

		assert( timerIt != timers.end() && "Timer does not exist." );

		if ( timerIt != timers.end() )
		{
			Timer& timer = timerIt->second;
			timer.active = false;
			timer.elapsedTime = 0.0f;
			timer.duration = firstDelay;
			timer.rate = rate;
			timer.loop = loop;
			timer.firstDelay = firstDelay;
			timer.callback = callback;

			return true;
		}

		return false;
	}

	void pauseTimer( TimerHandle handle )
	{
		auto timerIt = timers.find( handle );

		assert( timerIt != timers.end() && "Timer does not exist." );

		if ( timerIt != timers.end() )
		{
			Timer& timer = timerIt->second;
			timer.active = false;
		}
	}

	void startTimer( TimerHandle handle )
	{
		auto timerIt = timers.find( handle );

		assert( timerIt != timers.end() && "Timer does not exist." );

		if ( timerIt != timers.end() )
		{
			Timer& timer = timerIt->second;
			timer.active = true;
		}
	}

	void resetTimer( TimerHandle handle )
	{
		auto timerIt = timers.find( handle );

		assert( timerIt != timers.end() && "Timer does not exist." );

		if ( timerIt != timers.end() )
		{
			Timer& timer = timerIt->second;
			timer.active = false;
			timer.duration = timer.firstDelay;
			timer.elapsedTime = 0.0f;
		}
	}

	bool isTimerExists( TimerHandle handle ) const
	{
		return timers.count( handle ) > 0;
	}

	bool isTimerActive( TimerHandle handle ) const
	{
		auto timerIt = timers.find( handle );

		assert( timerIt != timers.end() && "Timer does not exist." );

		if ( timerIt != timers.end() )
		{
			const Timer& timer = timerIt->second;
			return timer.active;
		}

		return false;
	}

	float getTimerElapsed( TimerHandle handle ) const
	{
		auto timerIt = timers.find( handle );

		assert( timerIt != timers.end() && "Timer does not exist." );

		if ( timerIt != timers.end() )
		{
			const Timer& timer = timerIt->second;
			return timer.elapsedTime;
		}

		return -1.0f;
	}

	float getTimerRemaining( TimerHandle handle ) const
	{
		auto timerIt = timers.find( handle );

		assert( timerIt != timers.end() && "Timer does not exist." );

		if ( timerIt != timers.end() )
		{
			const Timer& timer = timerIt->second;
			return timer.duration - timer.elapsedTime;
		}

		return -1.0f;
	}

	void update( float deltaTime )
	{
		for ( auto& timerPair : timers )
		{
			Timer& timer = timerPair.second;

			if ( !timer.active ) continue;

			timer.elapsedTime += deltaTime;

			if ( timer.elapsedTime >= timer.duration )
            {
                timer.elapsedTime = timer.duration;

				if ( timer.loop )
				{
					timer.elapsedTime = 0.0f;
					timer.duration = timer.rate;
				}
				else
				{
					timer.active = false;
				}

				if ( timer.callback )
				{
					timer.callback();
				}
            }
		}
	}

private:

	std::unordered_map<TimerHandle, Timer> timers;
	TimerHandle nextTimerHandle = 0;
};


