// Oleg Kotov

#pragma once

#include "baseapi.h"

#include <stdint.h>
#include <vector>

class TextElement;
class RectangleElement;

class ENGINE_API UIRenderer
{
public:

	UIRenderer();
	~UIRenderer();

	void setDrawEnabled( bool enabled ) { m_drawEnabled = enabled; }
	bool isDrawEnabled() const { return m_drawEnabled; }

	void addText( TextElement* text );
	void removeText( TextElement* text );

	void addRectangle( RectangleElement* rect );

	void drawUI();

private:

	void flushVertexData() const;
	void bindVertexDataToShader() const;

	void drawRectangles() const;
	void drawTextElements() const;

private:

	bool m_drawEnabled = true;

	float m_rectangleVertices[8];

	uint32_t m_vertexArray = 0;
	uint32_t m_vertexBuffer = 0;

	class Shader* m_textShader = nullptr;
	class Shader* m_rectangleShader = nullptr;

	std::vector<TextElement*> m_texts;
	std::vector<RectangleElement*> m_rectangles;
};

