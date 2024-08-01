// Oleg Kotov

#pragma once

#include "asset.h"
#include "baseapi.h"

#include <stdint.h>

class ENGINE_API Sound : public Asset
{
public:

	Sound();
	~Sound();

	Asset::Type getAssetType() const override
	{
		return Asset::Type::Sound;
	}

	uint32_t getSoundId() const
	{
		return m_bufferHandle;
	}

private:

	uint32_t m_bufferHandle = 0;
};

