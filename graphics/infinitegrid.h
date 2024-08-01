// Oleg Kotov

#pragma once

#include "baseapi.h"
#include <cstdint>

class Shader;
class Texture;

class ENGINE_API InfiniteGrid
{
public:

	InfiniteGrid();
	~InfiniteGrid();

	void setDrawEnabled( bool enabled );
	bool isDrawEnabled() const;

	void drawGrid();

private:

	void flushVertexData() const;
	void bindVertexDataToShader() const;

private:

	bool m_drawEnabled = true;

	float m_gridPlaneVertices[18];

	uint32_t m_vertexArray = 0;
	uint32_t m_vertexBuffer = 0;

	Shader* m_gridShader = nullptr;
};

