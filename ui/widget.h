// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "matrix.h"
#include "layout_item.h"

#include <vector>

class Layout;
class Color;
class RectangleElement;

class ENGINE_API Widget : public LayoutItem
{
public:

	Widget( Widget* parent = nullptr )
	{

	}

	void doLayout() override
	{
		// do nothing?
	}

	virtual void onResolutionChanged( uint16_t width, uint16_t height ) {};
	virtual void draw() {};

	void setLayout( Layout* layout )
	{

	}

	void setBackgroundColor( const Color& color )
	{

	}

	void setWidth( int width )
	{
		m_width = width;
	}

	int getWidth()
	{
		return m_width;
	}

	void setHeight( int height )
	{
		m_height = height;
	}

	int getHeight()
	{
		return m_height;
	}

	Widget* m_parent;

	std::vector<Widget*> m_children;

	//
	Matrix m_transformationMatrix;

	// The position of the widget on the viewport along the X-axis in pixels
	int m_pos_x;

	// The position of the widget on the viewport along the Y-axis in pixels
	int m_pos_y;

	// The width of the widget in pixels
	int m_width;

	// The height of the widget in pixels
	int m_height;

	// The rotation angle of the widget in degrees
	float m_rotation;

	// The offset of the origin point along the X-axis relative to the widget's position
	float m_origin_x;

	// The offset of the origin point along the Y-axis relative to the widget's position
	float m_origin_y;

	RectangleElement* m_rectangle = nullptr;
};

