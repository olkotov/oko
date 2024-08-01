// Oleg Kotov

#pragma once

#include "baseapi.h"

class ENGINE_API LayoutItem
{
public:

	virtual ~LayoutItem() {}
	virtual void doLayout() = 0;

	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
};

