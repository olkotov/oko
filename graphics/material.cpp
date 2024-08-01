// Oleg Kotov

#include "material.h"
#include "log.h"

Material::Material( const std::string& name, Shader* shader /* = nullptr */ )
{
    m_name = name;
    m_shader = shader;
}

void Material::setName( const std::string& name )
{
	m_name = name;
}

const std::string& Material::getName() const
{
	return m_name;
}

void Material::setShader( Shader* shader )
{
	m_shader = shader;
}

Shader* Material::getShader() const
{
	return m_shader;
}

void Material::setDepthEnabled( bool enabled )
{
	m_depthEnabled = enabled;
}

bool Material::isDepthEnabled() const
{
	return m_depthEnabled;
}

void Material::setTransparent( bool transparent )
{
	m_transparent = transparent;
}

bool Material::isTransparent() const
{
	return m_transparent;
}

void Material::setDoubleSided( bool doubleSided )
{
	m_doubleSided = doubleSided;
}

bool Material::isDoubleSided() const
{
	return m_doubleSided;
}

void Material::setSurfaceType( SurfaceType surfaceType )
{
	m_surfaceType = surfaceType;
}

SurfaceType Material::getSurfaceType() const
{
	return m_surfaceType;
}

void Material::setRenderFace( RenderFace renderFace )
{
	m_renderFace = renderFace;
}

RenderFace Material::getRenderFace() const
{
	return m_renderFace;
}

void Material::setFloatParameter( const std::string& name, float value )
{
	for ( auto& param : m_floatParams )
	{
		if ( param.name == name )
		{
			param.value = value;
			return;
		}
	}

	m_floatParams.emplace_back( name, value );
}

bool Material::getFloatParameter( const std::string& name, float& outValue ) const
{
	for ( const auto& param : m_floatParams )
	{
		if ( param.name == name )
		{
			outValue = param.value;
			return true;
		}
	}

	Warning( "float parameter '%s' not found", name.c_str() );
	return false;
}

const std::vector<Material::FloatParameter>& Material::getFloatParameters() const
{
	return m_floatParams;
}

void Material::setVectorParameter( const std::string& name, const Vector& value )
{
	for ( auto& param : m_vectorParams )
	{
		if ( param.name == name )
		{
			param.value = value;
			return;
		}
	}

	m_vectorParams.emplace_back( name, value );
}

bool Material::getVectorParameter( const std::string& name, Vector& outValue ) const
{
	for ( const auto& param : m_vectorParams )
	{
		if ( param.name == name )
		{
			outValue = param.value;
			return true;
		}
	}

	Warning( "vector parameter '%s' not found", name.c_str() );
	return false;
}

const std::vector<Material::VectorParameter>& Material::getVectorParameters() const
{
	return m_vectorParams;
}

void Material::setColorParameter( const std::string& name, const Color& value )
{
	for ( auto& param : m_colorParams )
	{
		if ( param.name == name )
		{
			param.value = value;
			return;
		}
	}

	m_colorParams.emplace_back( name, value );
}

bool Material::getColorParameter( const std::string& name, Color& outValue ) const
{
	for ( const auto& param : m_colorParams )
	{
		if ( param.name == name )
		{
			outValue = param.value;
			return true;
		}
	}

	Warning( "color parameter '%s' not found", name.c_str() );
	return false;
}

const std::vector<Material::ColorParameter>& Material::getColorParameters() const
{
	return m_colorParams;
}

void Material::setTextureParameter( const std::string& name, Texture* value )
{
	for ( auto& param : m_textureParams )
	{
		if ( param.name == name )
		{
			param.value = value;
			return;
		}
	}

	m_textureParams.emplace_back( name, value );
}

bool Material::getTextureParameter( const std::string& name, Texture*& outValue ) const
{
	for ( const auto& param : m_textureParams )
	{
		if ( param.name == name )
		{
			outValue = param.value;
			return true;
		}
	}

	Warning( "texture parameter '%s' not found", name.c_str() );
	return false;
}

const std::vector<Material::TextureParameter>& Material::getTextureParameters() const
{
	return m_textureParams;
}

