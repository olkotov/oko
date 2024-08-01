// Oleg Kotov

#pragma once

#include "asset.h"
#include "baseapi.h"
#include <stdint.h>

class ENGINE_API Texture final : public Asset
{    
public:

    enum class Type : uint8_t
	{
		Texture2D,
		CubeTexture
	};

	enum class WrapMode : uint8_t
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class FilteringMethod : uint8_t // this is actually methods?
	{
		Nearest,
		NearestMipmapNearest,
		NearestMipmapLinear,
		Linear,
		LinearMipmapNearest,
		LinearMipmapLinear
	};

    // Texture::WrapMode WrapS = Texture::WrapMode::Repeat;
    // Texture::WrapMode WrapT = Texture::WrapMode::Repeat;
    // Texture::FilteringMethod MinFilter = Texture::FilteringMethod::LinearMipmapLinear;
    // Texture::FilteringMethod MagFilter = Texture::FilteringMethod::Linear;
    // int anisotropy;
    // int format;

public:

	Asset::Type getAssetType() const override
	{
		return Asset::Type::Texture;
	}

	void setTextureType( Texture::Type type )
	{
		m_textureType = type;
	}

	Texture::Type getTextureType() const
	{
		return m_textureType;
	}

	void setTextureId( uint32_t textureId )
	{
		m_textureId = textureId;
	}

	uint32_t getTextureId() const
	{
		return m_textureId;
	}

	int16_t getWidth() const
	{
		return m_width;
	}

	int16_t getHeight() const
	{
		return m_height;
	}

	int8_t getNumChannels() const
	{
		return m_numChannels;
	}

private:

	Texture::Type m_textureType = Texture::Type::Texture2D;
    uint32_t m_textureId = 0;

	int16_t m_width = 0;
	int16_t m_height = 0;
	int8_t m_numChannels = 0;

	friend class ResourceManager;
};

