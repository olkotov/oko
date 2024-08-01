// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "glyph.h"

#include <stdint.h>
#include <map>

class Texture;

class ENGINE_API Font
{
public:

	Font( Texture* atlas  )
	: m_textureAtlas( atlas )
	{}

	void addGlyph( Glyph* glyph )
	{
		m_glyphs.emplace( std::make_pair( glyph->getId(), glyph ) );
	}

	Glyph* getGlyph( int32_t id )
	{
		return m_glyphs[id];
	}

	void setSpaceWidth( uint16_t spaceWidth )
	{
		m_spaceWidth = spaceWidth;
	}

	uint16_t getSpaceWidth() const
	{
		return m_spaceWidth;
	}

	void setLineHeight( uint16_t lineHeight )
	{
		m_lineHeight = lineHeight;
	}

	uint16_t getLineHeight() const
	{
		return m_lineHeight;
	}

	Texture* getTextureAtlas() const
	{
		return m_textureAtlas;
	}

private:

	// The font texture atlas.
	Texture* m_textureAtlas = nullptr;

	// The glyph map.
	std::map<int32_t, Glyph*> m_glyphs;

	// The width of a "space" char in pixels.
	uint16_t m_spaceWidth = 0;
	
	// The height of a line in pixels.
	uint16_t m_lineHeight = 0;
};

/*

	https://wiki.rage.mp/index.php?title=Fonts_and_Colors
	"Зажмите ~u~левую кнопку мыши~w~, чтобы передвинуть камеру."

	"gameui_player_health" "Здоровье игрока: {hp}"

	label = localization()->getText( "gameui_player_health" );

	""

	textField.Text = textPattern.Replace( "{HP}", healthController.HP.ToString() );

*/

