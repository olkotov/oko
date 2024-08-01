// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "uielement.h"
#include "vector.h"
#include "shader.h"

#include "font.h"
#include "glyph.h"

#include "okoengine.h"
#include "uirenderer.h"

#include <gl/glew.h>

#include <stdint.h>
#include <unordered_map>

#include "fontloader.h"

struct GlyphVertex
{
	GlyphVertex( float x, float y, float u, float v, float r, float g, float b )
	{
		m_position[0] = x;
		m_position[1] = y;

		m_textureCoords[0] = u;
		m_textureCoords[1] = v;

		m_color[0] = r;
		m_color[1] = g;
		m_color[2] = b;
	}

	float m_position[2];
	float m_textureCoords[2];
	float m_color[3];
};

#define VERTEX_POSITION 0
#define VERTEX_TEXCOORD 1
#define VERTEX_COLOR    2

struct ColoredChar
{
	ColoredChar( Glyph* glyph, const Color& color )
	: m_glyph( glyph )
	, m_color( color )
	{}

	Glyph* m_glyph;
	Color m_color;
};

class Word
{
public:

	Word( float fontSize )
	: m_fontSize( fontSize )
	{}

	void addCharacter( Glyph* character, const Color& color )
	{
		m_characters.emplace_back( character, color );
		m_width += character->getAdvance() * m_fontSize;
	}

	float getWidth() const
	{
		return m_width;
	}

	const std::vector<ColoredChar>& getCharacters() const
	{
		return m_characters;
	}

private:

	float m_width = 0.0f;
	float m_fontSize = 1.0f;

	std::vector<ColoredChar> m_characters;
};

class Line
{
public:

	Line( uint16_t spaceWidth, float fontSize, uint16_t wrapWidth )
	: m_spaceWidth( spaceWidth * fontSize )
	, m_wrapWidth( wrapWidth )
	{}

	bool attemptToAddWord( const Word& word )
	{
		float additionalLength = word.getWidth();
		additionalLength += !m_words.empty() ? m_spaceWidth : 0.0f; // push space if other words exist in line

		if ( m_currentLength + additionalLength <= m_wrapWidth )
		{
			m_words.push_back( word );
			m_currentLength += additionalLength;
			
			return true;
		}

		return false;
	}

	float getMaxLength() const
	{
		return m_wrapWidth;
	}
	
	float getLength() const
	{
		return m_currentLength;
	}

	const std::vector<Word>& getWords() const
	{
		return m_words;
	}

private:

	uint16_t m_wrapWidth = 0;
	float m_currentLength = 0.0f;
	float m_spaceWidth = 0.0f;

	std::vector<Word> m_words;
};

enum class TextAlignment : uint8_t
{
	left,
	center,
	right
};

class ENGINE_API TextElement : public UIElement
{
public:

	TextElement()
	{
		glGenVertexArrays( 1, &m_vertexArray );
		glGenBuffers( 1, &m_vertexBuffer );

		createTextMesh();
		flushVertices();

		engine()->ui()->addText( this );
	}

	/*TextElement( const std::string& text, int16_t x, int16_t y, float fontSize, const Color& color, Font* font, uint16_t wrapWidth, TextAlignment alignment )
	{
		m_text = text;

		m_posx = x;
		m_posy = y;

		m_wrapWidth = wrapWidth;
		m_alignment = alignment;

		m_font = font;
		m_fontScale = fontSize;
		
		m_color = color;

		glGenVertexArrays( 1, &m_vertexArray );
		glGenBuffers( 1, &m_vertexBuffer );

		createTextMesh();
		flushVertices();

		engine()->ui()->addText( this );
	}*/

	~TextElement()
	{
		glDeleteBuffers( 1, &m_vertexBuffer );
		glDeleteVertexArrays( 1, &m_vertexArray );

		engine()->ui()->removeText( this );
	}

	void setText( const std::string& text )
	{
		m_text = text;

		createTextMesh();
		flushVertices();
	}

	const std::string& getText() const
	{
		return m_text;
	}

	void setPosition( int16_t x, int16_t y )
	{
		m_posx = x;
		m_posy = y;
	}

	Vector getPosition() const
	{
		return Vector( m_posx, m_posy, 0.0f );
	}

	void setWrapWidth( uint16_t wrapWidth )
	{
		m_wrapWidth = wrapWidth;

		createTextMesh();
		flushVertices();
	}

	uint16_t getWrapWidth() const
	{
		return m_wrapWidth;
	}

	void setTextAlignment( TextAlignment alignment )
	{
		m_alignment = alignment;

		createTextMesh();
		flushVertices();
	}

	TextAlignment getTextAlignment() const
	{
		return m_alignment;
	}

	void setFont( Font* font )
	{
		m_font = font;

		createTextMesh();
		flushVertices();
	}

	void setFont( const std::string& filename )
	{
		Font* font = FontLoader().getFont( filename );
		assert( font != nullptr );

		setFont( font );
	}

	const Font* getFont() const
	{
		return m_font;
	}

	void setFontScale( float scale )
	{
		m_fontScale = scale;

		createTextMesh();
		flushVertices();
	}

	float getFontScale() const
	{
		return m_fontScale;
	}

	void setColor( const Color& color )
	{
		m_color = color;

		createTextMesh();
		flushVertices();
	}

	const Color& getColor() const
	{
		return m_color;
	}

	float getHeigth() const
	{
		return m_numLines * ( m_font->getLineHeight() * m_fontScale + m_lineHeight ) - m_lineHeight;
	}

	float getWidth() const
	{
		return m_width;
	}

	/*void setElementWidth( uint16 width )
	{
		m_width = width;
	}

	void setElementHeigth( uint16 height )
	{
		m_heigth = heigth;
	}*/

	void setUseShadow( bool state )
	{
		m_useShadow = state;
	}

	bool isUseShadow() const
	{
		return m_useShadow;
	}

	void setSdfWidth( float width )
	{
		m_sdfWidth = width;
	}

	float getSdfWidth() const
	{
		return m_sdfWidth;
	}

	void setSdfEdge( float edge )
	{
		m_sdfEdge = edge;
	}

	float getSdfEdge() const
	{
		return m_sdfEdge;
	}

	void setLineHeight( int16_t lineHeight )
	{
		m_lineHeight = lineHeight;

		createTextMesh();
		flushVertices();
	}

	int16_t getLineHeight() const
	{
		return m_lineHeight;
	}

	Vector getTransform() const
	{
		return Vector( m_posx, m_posy, m_fontScale );
	}

	uint32_t getVertexArray() const
	{
		return m_vertexArray;
	}

	uint32_t getNumVertices() const
	{
		return m_numVertices;
	}

	std::vector<Line> createStructure()
	{
		const char* chars = m_text.c_str();

		std::vector<Line> lines;

		Line currentLine( m_font->getSpaceWidth(), m_fontScale, m_wrapWidth );
		Word currentWord( m_fontScale );

		for ( int32_t i = 0; i < m_text.size(); ++i )
		{
			if ( chars[i] == '\n' )
			{
				bool added = currentLine.attemptToAddWord( currentWord );

				lines.push_back( currentLine );
				currentLine = Line( m_font->getSpaceWidth(), m_fontScale, m_wrapWidth );

				currentWord = Word( m_fontScale );
				continue;
			}

			if ( chars[i] == '~' && chars[i + 2] == '~' )
			{
				// Msg( "token found, color: %c", chars[i + 1] );

				if ( chars[i + 1] == 'w' )
				{
					m_color = Color::White;
				}
				else if ( chars[i + 1] == 'y' )
				{
					m_color = Color( 240, 200, 80, 255 );
				}
				else if ( chars[i + 1] == 'r' )
				{
					m_color = Color( 224, 50, 50, 255 );
				}

				i += 2;
				continue;
			}

			if ( chars[i] == ' ' )
			{
				bool added = currentLine.attemptToAddWord( currentWord );

				if ( added == false )
				{
					lines.push_back( currentLine );
					currentLine = Line( m_font->getSpaceWidth(), m_fontScale, m_wrapWidth );
					currentLine.attemptToAddWord( currentWord );
				}

				currentWord = Word( m_fontScale );
				continue;
			}

			Glyph* glyph = m_font->getGlyph( (int32_t)chars[i] );
			currentWord.addCharacter( glyph, m_color );
		}

		completeStructure( lines, currentLine, currentWord );

		return lines;
	}

	void completeStructure( std::vector<Line>& lines, Line& currentLine, const Word& currentWord )
	{
		bool added = currentLine.attemptToAddWord( currentWord );

		if ( added == false )
		{
			lines.push_back( currentLine );
			currentLine = Line( m_font->getSpaceWidth(), m_fontScale, m_wrapWidth );
			currentLine.attemptToAddWord( currentWord );
		}

		lines.push_back( currentLine );
	}

	void addVerticesForCharacter( float cursorX, float cursorY, Glyph* character, float fontSize, const Color& color )
	{
		float x = cursorX + character->getOffsetX() * fontSize;
		float y = cursorY + character->getOffsetY() * fontSize;
		float maxX = x + character->getWidth() * fontSize;
		float maxY = y + character->getHeight() * fontSize;

		m_vertices.emplace_back( x, y, character->getTexCoordX(), character->getTexCoordY(), color.r, color.g, color.b );
		m_vertices.emplace_back( x, maxY, character->getTexCoordX(), character->getTexCoordMaxY(), color.r, color.g, color.b );
		m_vertices.emplace_back( maxX, maxY, character->getTexCoordMaxX(), character->getTexCoordMaxY(), color.r, color.g, color.b );
		m_vertices.emplace_back( maxX, maxY, character->getTexCoordMaxX(), character->getTexCoordMaxY(), color.r, color.g, color.b );
		m_vertices.emplace_back( maxX, y, character->getTexCoordMaxX(), character->getTexCoordY(), color.r, color.g, color.b );
		m_vertices.emplace_back( x, y, character->getTexCoordX(), character->getTexCoordY(), color.r, color.g, color.b );
	}

	void createTextMesh()
	{
		if ( !m_font ) return;

		std::vector<Line> lines = createStructure();

		// ------------

		m_numLines = lines.size();

		if ( m_numLines > 0 )
		{
			float width = 0.0f;

			for ( const auto& line : lines )
			{
				if ( line.getLength() > width )
				{
					width = line.getLength();
				}
			}

			m_width = width;
		}
		
		// ------------

		createQuadVertices( lines );
	}

	void createQuadVertices( const std::vector<Line>& lines )
	{		
		float cursorX = 0.0f;
		float cursorY = 0.0f;
		
		for ( const auto& line : lines )
		{
			cursorX = applyTextAlignment( line );
			
			for ( const auto& word : line.getWords() )
			{
				for ( const auto& character : word.getCharacters() )
				{					
					addVerticesForCharacter( cursorX, cursorY, character.m_glyph, m_fontScale, character.m_color );
					cursorX += character.m_glyph->getAdvance() * m_fontScale;
				}

				cursorX += m_font->getSpaceWidth() * m_fontScale;
			}

			cursorX = 0;
			cursorY += m_font->getLineHeight() * m_fontScale + m_lineHeight;
		}
	}

	float applyTextAlignment( const Line& line )
	{
		if ( m_alignment == TextAlignment::center )
		{
			return ( line.getMaxLength() - line.getLength() ) * 0.5f;
		}
		else if ( m_alignment == TextAlignment::right )
		{
			return line.getMaxLength() - line.getLength();
		}
		else // TextAlignment::left
		{
			return 0.0f;
		}
	}

	void flushVertices()
	{
		m_numVertices = m_vertices.size();

		if ( m_numVertices == 0 ) return;

		glBindVertexArray( m_vertexArray );
		glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );

		// glBufferData( GL_ARRAY_BUFFER, m_numVertices * sizeof( GlyphVertex ), &m_vertices[0], GL_STATIC_DRAW );

		uint32_t vertexBufferSize = m_numVertices * sizeof( GlyphVertex );

		if ( m_vertexBufferAllocSize < vertexBufferSize )
		{
			m_vertexBufferAllocSize = vertexBufferSize;
			glBufferData( GL_ARRAY_BUFFER, m_vertexBufferAllocSize, NULL, GL_STREAM_DRAW );
		}

		glBufferSubData( GL_ARRAY_BUFFER, 0, vertexBufferSize, &m_vertices[0] );

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindVertexArray( 0 );

		m_vertices.clear();
	}

	void bindToShader( Shader* shader ) const
	{
		glBindVertexArray( m_vertexArray );
		glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );

		GLint positionAttribute = glGetAttribLocation( shader->getNativeHandle(), "position" );
		glEnableVertexAttribArray( positionAttribute );
		glVertexAttribPointer(
			/* index     = */ positionAttribute,
			/* size      = */ 2,
			/* type      = */ GL_FLOAT,
			/* normalize = */ GL_FALSE,
			/* stride    = */ sizeof( GlyphVertex ),
			/* offset    = */ (GLvoid*)offsetof( GlyphVertex, m_position ) );

		GLint uvAttribute = glGetAttribLocation( shader->getNativeHandle(), "uv" );
		glEnableVertexAttribArray( uvAttribute );
		glVertexAttribPointer(
			/* index     = */ uvAttribute,
			/* size      = */ 2,
			/* type      = */ GL_FLOAT,
			/* normalize = */ GL_FALSE,
			/* stride    = */ sizeof( GlyphVertex ),
			/* offset    = */ (GLvoid*)offsetof( GlyphVertex, m_textureCoords ) );

		GLint colorAttribute = glGetAttribLocation( shader->getNativeHandle(), "color" );
		glEnableVertexAttribArray( colorAttribute );
		glVertexAttribPointer(
			/* index     = */ colorAttribute,
			/* size      = */ 3,
			/* type      = */ GL_FLOAT,
			/* normalize = */ GL_FALSE,
			/* stride    = */ sizeof( GlyphVertex ),
			/* offset    = */ (GLvoid*)offsetof( GlyphVertex, m_color ) );

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindVertexArray( 0 );
	}

private:

	std::string m_text;

	int16_t m_posx = 0;
	int16_t m_posy = 0;

	uint16_t m_wrapWidth = 500; // px
	TextAlignment m_alignment = TextAlignment::left;

	float m_width = m_wrapWidth;

	Font* m_font = nullptr;
	float m_fontScale = 1.0f;
	
	Color m_color = Color::White;

	// ---

	float m_sdfWidth = 0.4f;
	float m_sdfEdge = 0.25f;

	uint16_t m_numLines = 0;

	// Additional line height.
	int16_t m_lineHeight = 0;

	bool m_useShadow = false;

	// ---

	uint32_t m_numVertices = 0;
	std::vector<GlyphVertex> m_vertices;

	uint32_t m_vertexArray = 0;
	uint32_t m_vertexBuffer = 0;

	uint32_t m_vertexBufferAllocSize = 0;
};

