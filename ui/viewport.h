// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "layout.h"

#include <vector>

class ENGINE_API Viewport
{
public:
    Viewport( uint16_t width, uint16_t height )
        : m_width( width )
        , m_height( height )
        , m_rootLayout( nullptr )
    {}

    /*void setRootWidget( Widget* widget )
    {
        m_rootWidget = (widget) ? widget : nullptr;
    }*/

    void setLayout( Layout* layout )
    {
        if ( layout ) m_rootLayout = layout;
    }

	void doLayout()
	{
        m_rootLayout->doLayout();
	}

    //void draw()
    //{
    //    m_rootWidget->draw();
    //}

    //void onResolutionChanged( uint16_t width, uint16_t height )
    //{
    //    m_rootWidget->onResolutionChanged( width, height );
    //}

private:

    uint16_t m_width;
    uint16_t m_height;

    Layout* m_rootLayout;
};

