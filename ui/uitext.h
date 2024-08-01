// Oleg Kotov

#pragma once

#include "okobase.h"
#include "uielement.h"
#include "vector.h"

class Font;

class ENGINE_API UIText : public UIElement
{
public:

	UIText( const std::string& caption, const Vector& position, const Vector& scale, const Color& color, Font* font )
	{
		m_caption = caption;
		m_position = position;
		m_scale = scale;
		m_color = color;
		m_font = font;
	}

	const std::string& getCaption() const
	{
		return m_caption;
	}

	const Vector& getPosition() const
	{
		return m_position;
	}

	const Vector& getScale() const
	{
		return m_position;
	}

	const Color& getColor() const
	{
		return m_color;
	}

	const Font* getFont() const
	{
		return m_font;
	}

private:

	std::string m_caption;
	Vector m_position = Vector::zero;
	Vector m_scale = Vector::one;
	Color m_color = Color::White;
	Font* m_font = nullptr;
};

