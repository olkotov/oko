// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "matrix.h"

#include <cstdint>

class Shader;
class Texture;

class ENGINE_API Skybox
{
public:

	Skybox();
	~Skybox();

	void setDrawEnabled( bool enabled );
	bool isDrawEnabled() const;

	void setSkyTexture( Texture* texture );

	void drawSkybox();

private:

	void flushVertexData() const;
	void bindVertexDataToShader() const;

private:

	bool m_drawEnabled = true;

	float m_cubeVertices[108];

	uint32_t m_vertexArray = 0;
	uint32_t m_vertexBuffer = 0;

	Shader* m_skyboxShader = nullptr;
	Texture* m_skyboxTexture = nullptr;

	Matrix m_axisConversionMatrix;
};

