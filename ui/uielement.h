// Oleg Kotov

#pragma once

#include "baseapi.h"

class ENGINE_API UIElement
{
public:

	UIElement() = default;

	bool isEnabled() const
	{
		return m_enabled;
	}

private:

	bool m_enabled = true;
};

