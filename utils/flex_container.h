// Oleg Kotov

#pragma once

#include "rectangleelement.h"
#include "textelement.h"
#include "gameplay.h"
#include "event.h"
#include "okoengine.h"
#include "inputsystem.h"
#include "log.h"

using MouseEvent = Event<>;

enum class AxisDirection : uint8_t
{
	horizontal,
	vertical
};

enum class MainAxisAlignment : uint8_t
{
	start,
	end,
	center,
	spaceBetween,
	spaceAround,
	spaceEvenly
};

enum class CrossAxisAlignment : uint8_t
{
	start,
	end,
	center
};

enum class SizeMode : uint8_t
{
	fixedSize,
	fitContent,
	fitAvailable
};

class FlexContainer
{
public:

	FlexContainer()
	{
		rect = new RectangleElement();

		text = new TextElement();
		text->setFont( "fonts/inter-regular.fnt" );
		text->setFontScale( 0.3f );
		// text->setText( "Button" );

		m_pos_x = rect->getPositionX();
		m_pos_y = rect->getPositionY();

		// m_widthWithoutPaddings = rect->getWidth();
		// m_heightWithoutPaddings = rect->getHeight();

		m_widthWithoutPaddings = -1;
		m_heightWithoutPaddings = -1;
	}

	void setText( const std::string& text )
	{
		this->text->setText( text );
	}

	void addWidget( FlexContainer* widget )
	{
		widget->parent = this;
		children.push_back( widget );
	}

	void setBackgroundColor( const Color& color )
	{
		rect->setFillColor( color );
	}

	void setBorderRadius( float radius )
	{
		rect->setCornerRadius( radius );
	}

	void setPositionX( int pos )
	{
		m_pos_x = pos;
		rect->setPositionX( pos );
		text->setPosition( pos, text->getPosition().y );
	}

	void setPositionY( int pos )
	{
		m_pos_y = pos;
		rect->setPositionY( pos );
		text->setPosition( text->getPosition().x, pos );
	}

	void setWidth( int width )
	{
		m_widthSizeMode = SizeMode::fixedSize;
		m_widthWithoutPaddings = width;
		rect->setWidth( m_widthWithoutPaddings + padding_left + padding_right );
	}

	void setHeight( int height )
	{
		m_heightSizeMode = SizeMode::fixedSize;
		m_heightWithoutPaddings = height;
		rect->setHeight( m_heightWithoutPaddings + padding_top + padding_bottom );
	}

	void updateVisual()
	{
		rect->setWidth( m_widthWithoutPaddings + padding_left + padding_right );
		rect->setHeight( m_heightWithoutPaddings + padding_top + padding_bottom );
	}

	void setPaddings( int value )
	{
		padding_left = value;
		padding_top = value;
		padding_right = value;
		padding_bottom = value;

		updateVisual();
	}

	void setPaddings( int left, int top, int right, int bottom )
	{
		padding_left = left;
		padding_top = top;
		padding_right = right;
		padding_bottom = bottom;

		updateVisual();
	}

	void setPaddings( int vertical, int horizontal )
	{
		padding_left = horizontal;
		padding_top = vertical;
		padding_right = horizontal;
		padding_bottom = vertical;

		updateVisual();
	}

	void setAxisDirection( AxisDirection direction )
	{
		m_axisDirection = direction;
	}

	int getPosX() const
	{
		return m_pos_x;
	}

	int getPosY() const
	{
		return m_pos_y;
	}

	// actually calc width
	int getWidth()
	{
		if ( m_widthSizeMode == SizeMode::fitAvailable )
		{
			assert( parent );

			if ( parent->m_axisDirection == AxisDirection::horizontal )
			{
				// no division by zero, we already 1 stretch
				m_widthWithoutPaddings = parent->getAvailableWidth() / parent->getHorizontalStretchCount() - padding_left - padding_right;
			}
			else
			{
				// just use all height of parent
				m_widthWithoutPaddings = parent->getWidthWithoutPaddings() - padding_left - padding_right;
			}
		}

		if ( m_widthSizeMode == SizeMode::fitContent )
		{
			if ( children.empty() ) // only paddings
			{
				return padding_left + padding_right;
			}

			int childrenWidth = 0;

			for ( auto& child : children )
			{
				assert( child->m_widthSizeMode != SizeMode::fitAvailable );
				childrenWidth += child->getWidth();
			}

			m_widthWithoutPaddings = childrenWidth;
		}

		return m_widthWithoutPaddings + padding_left + padding_right;
	}

	int getWidthWithoutPaddings()
	{
		if ( m_widthSizeMode == SizeMode::fitAvailable )
		{
			assert( parent );
			m_widthWithoutPaddings = parent->getAvailableWidth() - padding_left - padding_right;
		}

		return m_widthWithoutPaddings;
	}
	
	int getHeight()
	{
		if ( m_heightSizeMode == SizeMode::fitAvailable )
		{
			assert( parent );

			if ( parent->m_axisDirection == AxisDirection::horizontal )
			{
				// just use all height of parent
				m_heightWithoutPaddings = parent->getHeightWithoutPaddings() - padding_top - padding_bottom;
			}
			else
			{
				// no division by zero, we already 1 stretch
				m_heightWithoutPaddings = parent->getAvailableHeight() / parent->getVerticalStretchCount() - padding_top - padding_bottom;
			}
		}

		if ( m_heightSizeMode == SizeMode::fitContent )
		{
			if ( children.empty() ) // only paddings
			{
				return padding_top + padding_bottom;
			}

			int childrenHeight = 0;

			for ( auto& child : children )
			{
				assert( child->m_heightSizeMode != SizeMode::fitAvailable );
				childrenHeight += child->getHeight();
			}

			m_heightWithoutPaddings = childrenHeight;
		}

		return m_heightWithoutPaddings + padding_top + padding_bottom;
	}

	int getHeightWithoutPaddings() const
	{
		return m_heightWithoutPaddings;
	}

	int getAvailableWidth()
	{
		int childrenWidth = 0;
		int width = getWidthWithoutPaddings();

		for ( FlexContainer* child : children )
		{
			if ( child->m_widthSizeMode == SizeMode::fixedSize )
			{
				childrenWidth += child->getWidth();
			}
		}

		return width - childrenWidth;
	}

	int getAvailableHeight()
	{
		int childrenHeight = 0;
		int height = getHeightWithoutPaddings();

		for ( FlexContainer* child : children )
		{
			if ( child->m_heightSizeMode == SizeMode::fixedSize )
			{
				childrenHeight += child->getHeight();
			}
		}

		return height - childrenHeight;
	}

	int getHorizontalStretchCount() const
	{
		int count = 0;

		for ( const auto& child : children )
		{
			if ( child->m_widthSizeMode != SizeMode::fixedSize ) count++;
		}

		return count;
	}

	int getVerticalStretchCount() const
	{
		int count = 0;

		for ( const auto& child : children )
		{
			if ( child->m_heightSizeMode != SizeMode::fixedSize ) count++;
		}

		return count;
	}

	void performLayout()
	{
		// calc width of element
		int width = getWidth();

		// calc height of element
		int height = getHeight();

		// update visual element
		updateVisual();

		// do layout

		if ( m_axisDirection == AxisDirection::horizontal )
		{
			performLayoutHorizontal();
		}
		else
		{
			performLayoutVertical();
		}

		// do it for children

		for ( auto& child : children )
		{
			child->performLayout();
		}
	}

	void performLayoutHorizontal()
	{
		int totalWidth = 0;
		int availableWidth = getWidthWithoutPaddings();

		for ( FlexContainer* child : children )
		{
			totalWidth += child->getWidth();
		}

		int remainingSpace = availableWidth - totalWidth;
		int spaceBetweenChildren = 0;
		int spaceAroundChildren = 0; // like padding

		// ---

		switch ( mainAxisAlignment )
		{
		case MainAxisAlignment::start:
			// do nothing
			break;

		case MainAxisAlignment::end:
			spaceAroundChildren = remainingSpace;
			break;

		case MainAxisAlignment::center:
			spaceAroundChildren = remainingSpace / 2;
			break;

		case MainAxisAlignment::spaceBetween:
			spaceBetweenChildren = remainingSpace / ( children.size() - 1 );
			break;

		case MainAxisAlignment::spaceAround:
			spaceAroundChildren = remainingSpace / ( children.size() * 2 );
			spaceBetweenChildren = spaceAroundChildren * 2;			
			break;

		case MainAxisAlignment::spaceEvenly:
			spaceBetweenChildren = remainingSpace / ( children.size() + 1 );
			spaceAroundChildren = spaceBetweenChildren;
			break;
		}

		// ---

		int currentX = m_pos_x + padding_left + spaceAroundChildren;

		for ( FlexContainer* child : children )
		{
			child->setPositionX( currentX );

			int pos_y = m_pos_y + padding_top;

			switch ( crossAxisAlignment )
			{
			case CrossAxisAlignment::start:
				// do nothing
				break;

			case CrossAxisAlignment::end:
				pos_y += ( getHeightWithoutPaddings() - child->getHeight() );
				break;

			case CrossAxisAlignment::center:
				pos_y += ( getHeightWithoutPaddings() - child->getHeight() ) * 0.5f;
				break;
			}

			child->setPositionY( pos_y );

			currentX += child->getWidth() + spaceBetweenChildren;
		}
	}

	void performLayoutVertical()
	{
		int totalHeight = 0;
		int availableHeight = getHeightWithoutPaddings();

		for ( FlexContainer* child : children )
		{
			totalHeight += child->getHeight();
		}

		int remainingSpace = availableHeight - totalHeight;
		int spaceBetweenChildren = 0;
		int spaceAroundChildren = 0; // like padding

		// ---

		switch ( mainAxisAlignment )
		{
		case MainAxisAlignment::start:
			// do nothing
			break;

		case MainAxisAlignment::end:
			spaceAroundChildren = remainingSpace;
			break;

		case MainAxisAlignment::center:
			spaceAroundChildren = remainingSpace / 2;
			break;

		case MainAxisAlignment::spaceBetween:
			spaceBetweenChildren = remainingSpace / ( children.size() - 1 );
			break;

		case MainAxisAlignment::spaceAround:
			spaceAroundChildren = remainingSpace / ( children.size() * 2 );
			spaceBetweenChildren = spaceAroundChildren * 2;			
			break;

		case MainAxisAlignment::spaceEvenly:
			spaceBetweenChildren = remainingSpace / ( children.size() + 1 );
			spaceAroundChildren = spaceBetweenChildren;
			break;
		}

		// ---

		int currentY = m_pos_y + padding_top + spaceAroundChildren;

		for ( FlexContainer* child : children )
		{
			child->setPositionY( currentY );

			int pos_x = m_pos_x + padding_left;

			switch ( crossAxisAlignment )
			{
			case CrossAxisAlignment::start:
				// do nothing
				break;

			case CrossAxisAlignment::end:
				pos_x += ( getWidthWithoutPaddings() - child->getWidth() );
				break;

			case CrossAxisAlignment::center:
				pos_x += ( getWidthWithoutPaddings() - child->getWidth() ) * 0.5f;
				break;
			}

			child->setPositionX( pos_x );

			currentY += child->getHeight() + spaceBetweenChildren;
		}
	}

	bool isCursorOver()
	{
		int x, y = 0;
		getCursorPosition( &x, &y );

		int minX = m_pos_x;
		int maxX = m_pos_x + getWidth();
		int minY = m_pos_y;
		int maxY = m_pos_y + getHeight();

		if ( x >= minX && x < maxX && y >= minY && y < maxY )
		{
			return true;
		}
		
		return false;
	}

	void dispatchEvents()
	{
		bool cursorOver = isCursorOver();

		if ( cursorOver )
		{
			if ( !m_cursorOverLastTime )
			{
				m_cursorOverLastTime = cursorOver;
				mouseEnterEvent();
			}
		}
		else
		{
			if ( m_cursorOverLastTime )
			{
				m_cursorOverLastTime = cursorOver;
				mouseLeaveEvent();
			}
		}

		// ---

		bool lmbPressed = engine()->inputSystem()->isKeyPressed( Input::Key::LeftMouseButton );

		if ( cursorOver && lmbPressed && !m_pressedLastTime )
		{
			Msg( "pressed" );
		}

		if ( !lmbPressed && m_pressedLastTime )
		{
			Msg( "released" );
		}

		if ( cursorOver && !lmbPressed && m_pressedLastTime )
		{
			// Msg( "clicked" );
			mouseClickEvent();
		}

		m_pressedLastTime = lmbPressed;
	}

	void update()
	{
		dispatchEvents();

		for ( auto& child : children )
		{
			child->update();
		}
	}

public:

	AxisDirection m_axisDirection = AxisDirection::horizontal;
	MainAxisAlignment mainAxisAlignment = MainAxisAlignment::start;
	CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::start;

	FlexContainer* parent = nullptr;
	std::vector<FlexContainer*> children;

	int m_pos_x;
	int m_pos_y;
	int m_widthWithoutPaddings;
	int m_heightWithoutPaddings;

	SizeMode m_widthSizeMode = SizeMode::fitAvailable;
	SizeMode m_heightSizeMode = SizeMode::fitAvailable;

	int padding_left = 0;
	int padding_right = 0;
	int padding_top = 0;
	int padding_bottom = 0;

	class RectangleElement* rect = nullptr;
	class TextElement* text = nullptr;

	bool m_cursorOverLastTime = false;
	bool m_pressedLastTime = false;

	MouseEvent mouseEnterEvent;
	MouseEvent mouseLeaveEvent;
	MouseEvent mouseClickEvent;
};

