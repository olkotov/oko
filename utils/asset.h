// Oleg Kotov

#pragma once

#include "baseapi.h"
#include <stdint.h>

class ENGINE_API Asset
{
public:

	enum class Type : uint8_t
	{
		Undefined,
		StaticMesh,
		SkeletalMesh,
		Skeleton,
		Texture,
		Shader,
		Material,
		Sound,
		CubeTexture,
		AnimSequence
	};

public:

	virtual ~Asset() {};
	virtual Asset::Type getAssetType() const = 0;
};

