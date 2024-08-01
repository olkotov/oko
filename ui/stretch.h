// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "layout_item.h"

class Stretch : public LayoutItem
{
public:

	void doLayout() override
	{
		// calc stretch length?
	}

	int getWidth() override
	{
		return length;
	}

	int getHeight() override
	{
		return length;
	}

public:

	int length;
};

