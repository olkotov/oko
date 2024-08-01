// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "vector.h"
#include "quaternion.h"

#include <stdint.h>
#include <assert.h>
#include <string>
#include <vector>

// Keyframe
// This class holds a time/value pair for an animation.

struct ENGINE_API VectorKeyframe
{
	float m_time;
	Vector m_value;
};

struct ENGINE_API QuaternionKeyframe
{
	float m_time;
	Quaternion m_value;
};

struct ENGINE_API EventKeyframe
{
	float m_time;
	std::string m_value; // event name (step_l, step_r, fire etc.)
};

struct ENGINE_API JointAnimChannel
{
	int32_t findPositionKeyframe( float animationTime ) const
	{
		int32_t numPositionKeys = m_positionKeyframes.size();

		assert( numPositionKeys > 0 );

		for ( int32_t i = 0; i < numPositionKeys - 1; ++i )
		{
			float t = m_positionKeyframes[i + 1].m_time;

			if ( animationTime < t )
			{
				return i;
			}
		}

		return 0;
	}

	int32_t findRotationKeyframe( float animationTime ) const
	{
		int32_t numRotationKeys = m_rotationKeyframes.size();

		assert( numRotationKeys > 0 );

		for ( int32_t i = 0; i < numRotationKeys - 1; ++i )
		{
			float t = m_rotationKeyframes[i + 1].m_time;

			if ( animationTime < t )
			{
				return i;
			}
		}

		return 0;
	}

	int32_t findScaleKeyframe( float animationTime ) const
	{
		int32_t numScaleKeys = m_scaleKeyframes.size();

		assert( numScaleKeys > 0 );

		for ( int32_t i = 0; i < numScaleKeys - 1; ++i )
		{
			float t = m_scaleKeyframes[i + 1].m_time;

			if ( animationTime < t )
			{
				return i;
			}
		}

		return 0;
	}

	std::string m_jointName;

	std::vector<VectorKeyframe> m_positionKeyframes; // animation track
	std::vector<QuaternionKeyframe> m_rotationKeyframes;
	std::vector<VectorKeyframe> m_scaleKeyframes;
	// eventKeyframes?
};

// animation is keyframe sequence

struct ENGINE_API Animation // AnimSequence
{
	const JointAnimChannel* getChannelByJointName( const std::string& jointName ) const
	{
		int32_t numChannels = m_channels.size();

		for ( int32_t i = 0; i < numChannels; ++i )
		{
			const JointAnimChannel* channel = &m_channels[i];

			if ( channel->m_jointName == jointName )
			{
				return channel;
			}
		}

		return nullptr;
	}

	std::string m_name;

	float m_numFrames; // duration
	float m_framerate; // frames per second

	std::vector<JointAnimChannel> m_channels;

	// jointChannels - 1 channel on 1 joint
	// positionsChannels - 1 channel on 1 object
};

