// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "layout.h"

class ENGINE_API Column : public Layout
{
	using Super = Layout;

public:

	void doLayout() override
	{
		// m_items
		
		// calc vertical align

		// child
		Super::doLayout();
	}
};

