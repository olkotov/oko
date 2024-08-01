// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "layout_item.h"
#include "stretch.h"
#include "widget.h"

#include <vector>

class ENGINE_API Layout : public LayoutItem
{	
public:

	void doLayout() override
	{
		for ( auto& item : m_items )
		{
			item->doLayout();
		}
	}

	void addLayout( Layout* layout )
	{
		if ( layout ) m_items.push_back( layout );
	}

	void addWidget( Widget* widget )
	{
		if ( widget ) m_items.push_back( widget );
	}

	void addStretch()
	{
		m_items.push_back( new Stretch() );
	}

private:

	std::vector<LayoutItem*> m_items;
};

