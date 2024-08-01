// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "vector.h"
#include "color.h"

#include <stdint.h>
#include <vector>
#include <string>

class Shader;
class Texture;

enum class SurfaceType : uint8_t
{
	opaque,
	transparent
};

enum class RenderFace : uint8_t
{
	front,
	back,
	both
};

class ENGINE_API Material
{
public:

	struct FloatParameter
	{
		FloatParameter( const std::string& name, float value )
		: name( name )
		, value( value )
		{}

		std::string name;
		float value;
	};

	struct VectorParameter
	{
		VectorParameter( const std::string& name, const Vector& value )
		: name( name )
		, value( value )
		{}

		std::string name;
		Vector value;
	};

	struct ColorParameter
	{
		ColorParameter( const std::string& name, const Color& value )
		: name( name )
		, value( value )
		{}

		std::string name;
		Color value;
	};

	struct TextureParameter
	{
		TextureParameter( const std::string& name, Texture* value )
		: name( name )
		, value( value )
		{}

		std::string name;
		Texture* value = nullptr;
	};

public:

    Material( const std::string& name, Shader* shader = nullptr );

	void setName( const std::string& name );
	const std::string& getName() const;

	void setShader( Shader* shader );
	Shader* getShader() const;


    void setDepthEnabled( bool enabled );
	bool isDepthEnabled() const;
	
	void setTransparent( bool transparent );
	bool isTransparent() const;
	
	void setDoubleSided( bool doubleSided );
    bool isDoubleSided() const;

	void setSurfaceType( SurfaceType surfaceType );
	SurfaceType getSurfaceType() const;

	void setRenderFace( RenderFace renderFace );
	RenderFace getRenderFace() const;


    void setFloatParameter( const std::string& name, float value );
    bool getFloatParameter( const std::string& name, float& outValue ) const;
	const std::vector<FloatParameter>& getFloatParameters() const;

	void setVectorParameter( const std::string& name, const Vector& value );
    bool getVectorParameter( const std::string& name, Vector& outValue ) const;
	const std::vector<VectorParameter>& getVectorParameters() const;

	void setColorParameter( const std::string& name, const Color& value );
    bool getColorParameter( const std::string& name, Color& outValue ) const;
	const std::vector<ColorParameter>& getColorParameters() const;

	void setTextureParameter( const std::string& name, Texture* value );
    bool getTextureParameter( const std::string& name, Texture*& outValue ) const;
	const std::vector<TextureParameter>& getTextureParameters() const;

private:

	std::string m_name;
	Shader* m_shader = nullptr;

	bool m_depthEnabled = true;
	bool m_transparent = false;
	bool m_doubleSided = false;

	SurfaceType m_surfaceType = SurfaceType::opaque;
	RenderFace m_renderFace = RenderFace::front;

	std::vector<FloatParameter> m_floatParams;
	std::vector<VectorParameter> m_vectorParams;
	std::vector<ColorParameter> m_colorParams;
	std::vector<TextureParameter> m_textureParams;
};

