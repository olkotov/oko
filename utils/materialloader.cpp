// Oleg Kotov

#include "materialloader.h"
#include "okoengine.h"
#include "resourcemanager.h"
#include "material.h"
#include "log.h"

#include <tinyxml2.h>
using namespace tinyxml2;

Vector MaterialLoader::stringToVector( const char* str )
{
	Vector vec;
	sscanf_s( str, "%f %f %f", &vec.x, &vec.y, &vec.z );
	return vec;
}

Color MaterialLoader::stringToColor( const char* str )
{
	uint8_t r, g, b = 0;
	float a = 1.0f;

	sscanf_s( str, "%hhu %hhu %hhu %f", &r, &g, &b, &a );
	return Color( r, g, b, a );
}

Material* MaterialLoader::load( const std::string& filepath )
{
    tinyxml2::XMLDocument document;

	if ( document.LoadFile( filepath.c_str() ) != XML_SUCCESS )
	{
		Warning( "something went wrong" );
		return nullptr;
	}

	XMLElement* rootElement = document.RootElement();

	const char* shaderName = rootElement->Attribute( "shader" );

	Shader* shader = engine()->resources()->getShader( shaderName );

	Material* material = new Material( filepath, shader );

	XMLElement* element = rootElement->FirstChildElement();

	while ( element )
	{
		const char* paramName  = element->Attribute( "name" );
		const char* paramType  = element->Attribute( "type" );
		const char* paramValue = element->Attribute( "value" );

		if ( strcmp( paramType, "texture" ) == 0 )
		{
			Texture* texture = engine()->resources()->getTexture( paramValue );
			material->setTextureParameter( paramName, texture );
		}

		/*else if ( strcmp( paramType, "cubetexture" ) == 0 )
		{
			Texture* cubeTexture = engine()->resources()->getCubeTexture( paramValue );
			material->setTextureParameter( paramName, cubeTexture );
		}*/

		else if ( strcmp( paramType, "color" ) == 0 )
		{
			Color color = stringToColor( paramValue );
			material->setColorParameter( paramName, color );
		}

		else if ( strcmp( paramType, "vector" ) == 0 )
		{
			Vector vector = stringToVector( paramValue );
			material->setVectorParameter( paramName, vector );
		}

		else if ( strcmp( paramType, "float" ) == 0 )
		{
			float value = atof( paramValue );
			material->setFloatParameter( paramName, value );
		}

		else if ( strcmp( paramType, "bool" ) == 0 )
		{
			float value = ( strcmp( paramValue, "true" ) == 0 ) ? 1.0f : 0.0f;
			material->setFloatParameter( paramName, value );
		}

		else
		{
			Warning( "undefined parameter type" );
		}

		element = element->NextSiblingElement();
	}

	return material;
}

