// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "matrix.h"

#include <stdint.h>
#include <vector>

class Shader;
class Texture;

class ENGINE_API IBLMapGen
{        
public:
    
	IBLMapGen();
	~IBLMapGen();

	void flushCubeVertexData() const;
	void bindCubeVertexDataToShader( Shader* shader ) const;

	void flushQuadVertexData() const;
	void bindQuadVertexDataToShader( Shader* shader ) const;

	void genEnvironmentCubeMap();

	Texture* getEnvironmentCubeMap() const
	{
		return m_envTexture;
	}

	void genIrradianceCubeMap();

	Texture* getIrradianceCubeMap() const
	{
		return m_irradianceMapTex;
	}

	void genPrefilterCubeMap();

	Texture* getPrefilterCubeMap()
	{
		return m_prefilterMapTex;
	}

	void genBrdfLUTMap();

	Texture* getBrdfLUTMap()
	{
		return m_brdfLUTTextureTex;
	}

private:

	uint32_t m_captureFBO = 0;
	uint32_t m_captureRBO = 0;

	Matrix m_captureProjection;
	Matrix m_captureViews[6];

	// sky
	Texture* m_envTexture = nullptr;
	uint32_t m_envCubemap = 0;

	Shader* m_equirectangularToCubemapShader = nullptr;
	Texture* m_hdrTexture = nullptr;

	// ---

	uint16_t m_irradianceMapSize = 32; // 32

	Texture* m_irradianceMapTex = nullptr;
	uint32_t m_irradianceMap;

	Shader* m_irradianceShader = nullptr;

	// ---

	uint16_t m_prefilterMapSize = 128; // 128

	Texture* m_prefilterMapTex = nullptr;
	uint32_t m_prefilterMap;

	Shader* m_prefilterShader = nullptr;

	// ---

	uint16_t m_brdfLUTTextureSize = 512;

	Texture* m_brdfLUTTextureTex = nullptr;
	uint32_t m_brdfLUTTexture = 0;

	Shader* m_brdfShader = nullptr;

private:

	float m_cubeVertices[108];

	uint32_t m_cubeVertexArray = 0;
	uint32_t m_cubeVertexBuffer = 0;

	float m_quadVertices[20];

	uint32_t m_quadVertexArray = 0;
	uint32_t m_quadVertexBuffer = 0;

	Matrix m_axisConversionMatrix;
};

