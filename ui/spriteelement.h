// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "uielement.h"
#include "vector.h"

#include "okoengine.h"
#include "graphics.h"

#include <stdint.h>

class Texture;

class ENGINE_API SpriteElement : public UIElement
{
public:

	SpriteElement( const Vector& position, const Vector& scale, Texture* texture, float progress, float angle )
	{
		m_width = scale.x;
		m_height = scale.y;

		int32_t width = engine()->graphics()->getWindowWidth();
		int32_t height = engine()->graphics()->getWindowHeight();

		m_scale.x = scale.x * progress / width;
		m_scale.y = scale.y / height;

		// from https://www.youtube.com/watch?v=vOmJ1lyiJ4A comment by @rehcubprivat4815
		// m_position.x =   ( 2.0f * position.x / width + m_scale.x ) - 1.0f;
		// m_position.y = ( ( 2.0f * position.y / height  + m_scale.y ) - 1.0f ) * -1.0f;

		// from screenToWorld - screen to NormalizedDeviceCoords
		// good ref: https://stackoverflow.com/questions/65859056/draw-a-2d-quad-in-opengl
		m_position.x = ( position.x / width ) * 2.0f - 1.0f + m_scale.x;
		m_position.y = 1.0f - ( ( position.y / height ) * 2.0f + m_scale.y );

		m_texture = texture;
		m_progress = progress;

		m_rotationAngle = angle;
	}

	const Vector& getPosition() const
	{
		return m_position;
	}

	const Vector& getScale() const
	{
		return m_scale;
	}

	const Texture* getTexture() const
	{
		return m_texture;
	}

	float getProgress() const
	{
		return m_progress;
	}

	float getWidth() const
	{
		return m_width;
	}

	float getHeight() const
	{
		return m_height;
	}

	float getRotationAngle() const
	{
		return m_rotationAngle;
	}

private:

	float m_width = 0.0f;
	float m_height = 0.0f;

	Vector m_position = Vector::zero;
	Vector m_scale = Vector::one;
	Texture* m_texture = nullptr;
	float m_progress = 1.0f;

	float m_rotationAngle = 0.0f;
};

