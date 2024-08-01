// Oleg Kotov

#include "timelinecomponent.h"
#include "log.h"

void TimelineComponent::tick( float deltaTime )
{
	Super::tick( deltaTime );

	if ( !m_isPlaying ) return;

	if ( m_isReverse )
	{
		m_elapsedTime -= deltaTime;
	}
	else
	{
		m_elapsedTime += deltaTime;
	}

	m_elapsedTime = clamp( m_elapsedTime, 0.0f, m_duration );

	float t = m_elapsedTime / m_duration;
	// t = easeOutElastic( t );

	m_currentValue = lerp( 0.0f, 1.0f, t );

	updateEvent( m_currentValue );

	/*if ( m_callback )
	{
		m_callback( m_currentValue );
	}*/

	if ( m_elapsedTime == 0.0f || m_elapsedTime == m_duration )
	{
		m_isPlaying = false;
		finishedEvent();
	}
}

void TimelineComponent::play()
{
	// Msg( "play" );

	m_isReverse = false;
	m_isPlaying = true;
}

void TimelineComponent::playFromStart()
{
	m_elapsedTime = 0.0f;
	play();
}

void TimelineComponent::reverse()
{
	// Msg( "reverse" );

	m_isReverse = true;
	m_isPlaying = true;
}

void TimelineComponent::reverseFromEnd()
{
	m_elapsedTime = m_duration;
	reverse();
}

void TimelineComponent::stop()
{
	m_isPlaying = false;
}

void TimelineComponent::setDuration( float duration )
{
	if ( duration <= 0.0f ) duration = 0.0001f;
	m_duration = duration;
}

//void TimelineComponent::bind( TimelineCallback callback )
//{
//	m_callback = callback;
//}

