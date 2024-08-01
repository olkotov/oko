// Oleg Kotov

#pragma once

#include "baseapi.h"
#include <stdint.h>

class ENGINE_API Glyph
{
public:

	Glyph( uint32_t id, float x, float y, float u, float v, uint16_t offsetX, int16_t offsetY, int16_t width, uint16_t height, uint16_t advance )
		: m_id( id )
		, m_u( x )
		, m_v( y )
		, m_offsetX( offsetX )
		, m_offsetY( offsetY )
		, m_width( width )
		, m_height( height )
		, m_texCoordMaxX( u + x )
		, m_texCoordMaxY( v + y )
		, m_advance( advance )
	{}

	uint32_t getId() const
	{
		return m_id;
	}

	int16_t getOffsetX() const
	{
		return m_offsetX;
	}

	int16_t getOffsetY() const
	{
		return m_offsetY;
	}

	uint16_t getWidth() const
	{
		return m_width;
	}

	uint16_t getHeight() const
	{
		return m_height;
	}

	float getTexCoordX() const
	{
		return m_u;
	}

	float getTexCoordY() const
	{
		return m_v;
	}

	float getTexCoordMaxX() const
	{
		return m_texCoordMaxX;
	}

	float getTexCoordMaxY() const
	{
		return m_texCoordMaxY;
	}

	uint16_t getAdvance() const
	{
		return m_advance;
	}

private:

	// int32 m_id;
	// int32 m_width;
	// int32 m_height;
	// int32 m_bearingX;
	// int32 m_bearingY;
	// int32 m_advance;

	// The ASCII value of the character.
	uint32_t m_id;
	
	// The x texture coordinate for the top left corner of the character in the texture atlas.
	float m_u;
	
	// The y texture coordinate for the top left corner of the character in the texture atlas.
	float m_v;
	
	// The width of the character in the texture atlas.
	float m_texCoordMaxX;
	
	// The height of the character in the texture atlas.
	float m_texCoordMaxY;
	
	// The x distance from the cursor to the left edge of the character's quad.
	int16_t m_offsetX;
	
	// The y distance from the cursor to the top edge of the character's quad.
	int16_t m_offsetY;
	
	// The width of the character's quad in screen space.
	uint16_t m_width;
	
	// The height of the character's quad in screen space.
	uint16_t m_height;

	// How far in pixels the cursor should advance after adding this character.
	uint16_t m_advance;
};

