// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "font.h"
#include "glyph.h"

// hmm...
#include "okoengine.h"
#include "resourcemanager.h"

#include <stdint.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>

#include <filesystem>
namespace fs = std::filesystem;

template <typename Out>
void split( const std::string &s, char delim, Out result )
{
    std::istringstream iss( s );
    std::string item;

    while ( std::getline( iss, item, delim ) )
	{
        *result++ = item;
    }
}

ENGINE_API std::vector<std::string> split( const std::string &s, char delim );
ENGINE_API std::vector<std::string> split_no_quotes( const std::string& str );

class ENGINE_API FontLoader
{
public:

	enum FontPadding
	{
		FontPadding_Top,
		FontPadding_Left,
		FontPadding_Bottom,
		FontPadding_Right
	};
		
	const uint32_t ASCII_SPACE = 32;
	const uint8_t DESIRED_PADDING = 8;
	
	std::vector<int32_t> padding;
	uint16_t paddingWidth;
	uint16_t paddingHeight;
	
	std::ifstream file;
	std::map<std::string, std::string> values;

int32_t getValueOfVariable( const std::string& str )
{
	return std::stoi( values[str] );
}

std::vector<int32_t> getValuesOfVariable( const std::string& str )
{
	std::vector<std::string> numbers = split( values[str], ',' );

	std::vector<int32_t> actualValues;
	actualValues.reserve( numbers.size() );

	for ( const auto& number : numbers )
	{
		actualValues.push_back( std::stoi( number ) );
	}

	return actualValues;
}

bool processNextLine()
{
	values.clear();

	std::string line;
	std::getline( file, line );

	if ( line.size() == 0 )
	{
		return false;
	}

	for ( const auto& part : split( line, ' ' ) )
	{
		std::vector<std::string> valuePairs = split( part, '=' );

		if ( valuePairs.size() == 2 )
		{
			values.insert( std::make_pair( valuePairs[0], valuePairs[1] ) );
		}
	}

	return true;
}

void loadPaddingData()
{
	processNextLine();
	padding = getValuesOfVariable( "padding" );
	paddingWidth = padding[FontPadding_Left] + padding[FontPadding_Right];
	paddingHeight = padding[FontPadding_Top] + padding[FontPadding_Bottom];
}

void loadLineHeight( Font* font )
{
	processNextLine();
	uint16_t lineHeight = getValueOfVariable( "lineHeight" ) - paddingHeight;

	m_scaleConvertion = DESIRED_LINE_HEIGHT / (float)lineHeight;
	m_scaleConvertion = 1.0f;

	font->setLineHeight( lineHeight ); // in pixels
}

Glyph* loadCharacter( int32_t imageSize, Font* font )
{
	uint32_t id = getValueOfVariable( "id" );

	if ( id == ASCII_SPACE )
	{
		font->setSpaceWidth( ( getValueOfVariable( "xadvance" ) - paddingWidth ) * m_scaleConvertion );
		return nullptr;
	}

	float x = ( (float)getValueOfVariable( "x" ) + ( padding[FontPadding_Left] - DESIRED_PADDING ) ) / imageSize;
	float y = ( (float)getValueOfVariable( "y" ) + ( padding[FontPadding_Top] - DESIRED_PADDING ) ) / imageSize;

	uint16_t width = getValueOfVariable( "width" ) - ( paddingWidth - ( 2 * DESIRED_PADDING ) );
	uint16_t height = getValueOfVariable( "height" ) - ( paddingHeight - ( 2 * DESIRED_PADDING ) );

	double quadWidth = width * m_scaleConvertion;
	double quadHeight = height * m_scaleConvertion;

	float u = (float)width / imageSize;  // u? // why only single side of image?
	float v = (float)height / imageSize; // v?

	int16_t offsetX = ( getValueOfVariable( "xoffset" ) + padding[FontPadding_Left] - DESIRED_PADDING ) * m_scaleConvertion;
	int16_t offsetY = ( getValueOfVariable( "yoffset" ) + padding[FontPadding_Top] - DESIRED_PADDING ) * m_scaleConvertion;

	uint16_t advance = ( getValueOfVariable( "xadvance" ) - paddingWidth ) * m_scaleConvertion;

	return new Glyph( id, x, y, u, v, offsetX, offsetY, quadWidth, quadHeight, advance );
}

void loadCharacterData( int32_t imageWidth, Font* font )
{
	processNextLine();
	processNextLine();

	int32_t numGlyphs = getValueOfVariable( "count" );

	for ( int32_t i = 0; i < numGlyphs; ++i )
	{
		processNextLine();

		Glyph* glyph = loadCharacter( imageWidth, font );

		if ( glyph != nullptr )
		{
			font->addGlyph( glyph );
		}
	}
}

Font* getFont( const std::string& filename )
{
	fs::path fullPath( filename );
	std::string pathWithoutExtension = fullPath.parent_path().string() + "/" + fullPath.stem().string();

	Texture* texture = engine()->resources()->getTexture( pathWithoutExtension + "_sdf_atlas.png" ); // filename_sdf_atlas.png
	Font* font = new Font( texture );

	const std::string& contentPath = engine()->resources()->getContentDirectory();

	// file.open( contentPath + filename + ".fnt" );
	file.open( contentPath + filename );
	loadPaddingData();
	loadLineHeight( font );
	uint16_t imageWidth = getValueOfVariable( "scaleW" );
	loadCharacterData( imageWidth, font );

	return font;
}

	// Desired height of a line of text at font size 1 (ui scale 1).
	const uint8_t DESIRED_LINE_HEIGHT = 25;

	//
	float m_scaleConvertion = 25.0f;

};

