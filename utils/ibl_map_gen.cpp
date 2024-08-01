// Oleg Kotov

#include "ibl_map_gen.h"
#include "shader.h"
#include "texture.h"
#include "mathutils.h"
#include "perspectivematrix.h"
#include "lookatmatrix.h"
#include "okoengine.h"
#include "resourcemanager.h"

#include <GL/glew.h>

IBLMapGen::IBLMapGen()
	: m_cubeVertices
	{
		// positions

		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f
	}
	, m_quadVertices
	{
	// positions        // uv
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	}
	, m_captureViews
	{
	   LookAtMatrix( Vector( 0.0f, 0.0f, 0.0f ), Vector(  1.0f,  0.0f,  0.0f ), Vector( 0.0f, -1.0f,  0.0f ) ),
	   LookAtMatrix( Vector( 0.0f, 0.0f, 0.0f ), Vector( -1.0f,  0.0f,  0.0f ), Vector( 0.0f, -1.0f,  0.0f ) ),
	   LookAtMatrix( Vector( 0.0f, 0.0f, 0.0f ), Vector(  0.0f,  1.0f,  0.0f ), Vector( 0.0f,  0.0f,  1.0f ) ),
	   LookAtMatrix( Vector( 0.0f, 0.0f, 0.0f ), Vector(  0.0f, -1.0f,  0.0f ), Vector( 0.0f,  0.0f, -1.0f ) ),
	   LookAtMatrix( Vector( 0.0f, 0.0f, 0.0f ), Vector(  0.0f,  0.0f,  1.0f ), Vector( 0.0f, -1.0f,  0.0f ) ),
	   LookAtMatrix( Vector( 0.0f, 0.0f, 0.0f ), Vector(  0.0f,  0.0f, -1.0f ), Vector( 0.0f, -1.0f,  0.0f ) )
	}
{
	glGenVertexArrays( 1, &m_cubeVertexArray );
    glGenBuffers( 1, &m_cubeVertexBuffer );

	glGenVertexArrays( 1, &m_quadVertexArray );
    glGenBuffers( 1, &m_quadVertexBuffer );

	glGenFramebuffers(1, &m_captureFBO );
	glGenRenderbuffers(1, &m_captureRBO );

	glBindFramebuffer( GL_FRAMEBUFFER, m_captureFBO );
	glBindRenderbuffer( GL_RENDERBUFFER, m_captureRBO );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512 );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_captureRBO );

	// ---
	// Environment Cube Map

	glGenTextures( 1, &m_envCubemap );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_envCubemap );
	
	for ( int32_t i = 0; i < 6; ++i )
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr );
	}

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	m_envTexture = new Texture();
	m_envTexture->setTextureId( m_envCubemap );
	m_envTexture->setTextureType( Texture::Type::CubeTexture );

	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

	// ---
	// Diffuse IBL Cube Map

	glGenTextures( 1, &m_irradianceMap );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_irradianceMap );

	for ( uint32_t i = 0; i < 6; ++i )
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, m_irradianceMapSize, m_irradianceMapSize, 0, GL_RGB, GL_FLOAT, nullptr );
	}

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	m_irradianceMapTex = new Texture();
	m_irradianceMapTex->setTextureId( m_irradianceMap );
	m_irradianceMapTex->setTextureType( Texture::Type::CubeTexture );

	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

	// ---
	// Specular IBL Cube Map

	glGenTextures( 1, &m_prefilterMap );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_prefilterMap );

	for ( uint32_t i = 0; i < 6; ++i )
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr );
	}

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glGenerateMipmap( GL_TEXTURE_CUBE_MAP );

	m_prefilterMapTex = new Texture();
	m_prefilterMapTex->setTextureId( m_prefilterMap );
	m_prefilterMapTex->setTextureType( Texture::Type::CubeTexture );

	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

	// ---

	glGenTextures( 1, &m_brdfLUTTexture );
	glBindTexture( GL_TEXTURE_2D, m_brdfLUTTexture );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0 );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	m_brdfLUTTextureTex = new Texture();
	m_brdfLUTTextureTex->setTextureId( m_brdfLUTTexture );
	m_brdfLUTTextureTex->setTextureType( Texture::Type::Texture2D );

	glBindTexture( GL_TEXTURE_2D, 0 );

	// ---

	flushCubeVertexData();
	flushQuadVertexData();

	m_equirectangularToCubemapShader = engine()->resources()->getShader( "shaders/ibl/a" );
	m_irradianceShader = engine()->resources()->getShader( "shaders/ibl/b" );
	m_prefilterShader = engine()->resources()->getShader( "shaders/ibl/c" );
	m_brdfShader = engine()->resources()->getShader( "shaders/ibl/d" );

	m_hdrTexture = engine()->resources()->getTexture( "textures/environment/Arches_E_PineTree_3k.hdr" );

	m_captureProjection = PerspectiveMatrix( 90.0f * Deg2Rad, 1.0f, 0.1f, 10.0f );
	m_axisConversionMatrix = Matrix::makeRotationAxis( Vector( 1.0f, 0.0f, 0.0f ), 90.0f );
}

IBLMapGen::~IBLMapGen()
{
	glDeleteBuffers( 1, &m_cubeVertexBuffer );
    glDeleteVertexArrays( 1, &m_cubeVertexArray );

	glDeleteBuffers( 1, &m_quadVertexBuffer );
    glDeleteVertexArrays( 1, &m_quadVertexArray );
}

void IBLMapGen::flushCubeVertexData() const
{
	glBindVertexArray( m_cubeVertexArray );
    
	glBindBuffer( GL_ARRAY_BUFFER, m_cubeVertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( m_cubeVertices ), &m_cubeVertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void IBLMapGen::bindCubeVertexDataToShader( Shader* shader ) const
{
	glBindVertexArray( m_cubeVertexArray );
    glBindBuffer( GL_ARRAY_BUFFER, m_cubeVertexBuffer );
    
    GLint positionAttribute = glGetAttribLocation( shader->getNativeHandle(), "position" );
	glEnableVertexAttribArray( positionAttribute );
	glVertexAttribPointer(
		/* index     = */ positionAttribute,
		/* size      = */ 3,
		/* type      = */ GL_FLOAT,
		/* normalize = */ GL_FALSE,
		/* stride    = */ 3 * sizeof( float ),
		/* offset    = */ (void*)0 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void IBLMapGen::flushQuadVertexData() const
{
	glBindVertexArray( m_quadVertexArray );
    
	glBindBuffer( GL_ARRAY_BUFFER, m_quadVertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( m_quadVertices ), &m_quadVertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void IBLMapGen::bindQuadVertexDataToShader(Shader* shader) const
{
	glBindVertexArray( m_quadVertexArray );
    glBindBuffer( GL_ARRAY_BUFFER, m_quadVertexBuffer );
    
    GLint positionAttribute = glGetAttribLocation( shader->getNativeHandle(), "position" );
	glEnableVertexAttribArray( positionAttribute );
	glVertexAttribPointer(
		/* index     = */ positionAttribute,
		/* size      = */ 3,
		/* type      = */ GL_FLOAT,
		/* normalize = */ GL_FALSE,
		/* stride    = */ 5 * sizeof( float ),
		/* offset    = */ (void*)0 );

	GLint uvAttribute = glGetAttribLocation( shader->getNativeHandle(), "uv" );
	glEnableVertexAttribArray( uvAttribute );
	glVertexAttribPointer(
		/* index     = */ uvAttribute,
		/* size      = */ 2,
		/* type      = */ GL_FLOAT,
		/* normalize = */ GL_FALSE,
		/* stride    = */ 5 * sizeof( float ),
		/* offset    = */ (void*)( 3 * sizeof( float ) ) );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void IBLMapGen::genEnvironmentCubeMap()
{
	m_equirectangularToCubemapShader->bind();
	bindCubeVertexDataToShader( m_equirectangularToCubemapShader );

	static const int8_t textureSlot = 0;
	glActiveTexture( GL_TEXTURE0 + textureSlot );
	glBindTexture( GL_TEXTURE_2D, m_hdrTexture->getTextureId() );
	m_equirectangularToCubemapShader->setIntegerUniform( "_EquirectangularMap", textureSlot );

	glViewport( 0, 0, 512, 512 );
	glBindFramebuffer( GL_FRAMEBUFFER, m_captureFBO );

	for ( uint32_t i = 0; i < 6; ++i )
	{
		Matrix viewProjectionMatrix = m_captureProjection * m_captureViews[i] * m_axisConversionMatrix;
		m_equirectangularToCubemapShader->setMatrixUniform( "_ViewProjectionMatrix", viewProjectionMatrix.transposed() );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_envCubemap, 0 );
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		static const uint8_t vertexCount = 36;
		glBindVertexArray( m_cubeVertexArray );
		glDrawArrays( GL_TRIANGLES, 0, vertexCount );
		glBindVertexArray( 0 );
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void IBLMapGen::genIrradianceCubeMap()
{
	glBindFramebuffer( GL_FRAMEBUFFER, m_captureFBO );
	glBindRenderbuffer( GL_RENDERBUFFER, m_captureRBO );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_irradianceMapSize, m_irradianceMapSize );

	m_irradianceShader->bind();
	bindCubeVertexDataToShader( m_irradianceShader );

	static const int8_t textureSlot = 0;
	glActiveTexture( GL_TEXTURE0 + textureSlot );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_envCubemap );
	m_equirectangularToCubemapShader->setIntegerUniform( "_EnvironmentMap", textureSlot );

	glViewport( 0, 0, m_irradianceMapSize, m_irradianceMapSize );
	glBindFramebuffer( GL_FRAMEBUFFER, m_captureFBO );

	for ( unsigned int i = 0; i < 6; ++i )
	{
		Matrix viewProjectionMatrix = m_captureProjection * m_captureViews[i];
		m_irradianceShader->setMatrixUniform( "_ViewProjectionMatrix", viewProjectionMatrix.transposed() );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_irradianceMap, 0 );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		static const uint8_t vertexCount = 36;
		glBindVertexArray( m_cubeVertexArray );
		glDrawArrays( GL_TRIANGLES, 0, vertexCount );
		glBindVertexArray( 0 );
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void IBLMapGen::genPrefilterCubeMap()
{
	m_prefilterShader->bind();
	bindCubeVertexDataToShader( m_prefilterShader );

	static const int8_t textureSlot = 0;
	glActiveTexture( GL_TEXTURE0 + textureSlot );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_envCubemap );
	m_equirectangularToCubemapShader->setIntegerUniform( "_EnvironmentMap", textureSlot );

	glBindFramebuffer( GL_FRAMEBUFFER, m_captureFBO );
	uint8_t maxMipLevels = 5;

	for ( uint8_t mip = 0; mip < maxMipLevels; ++mip )
	{
		unsigned int mipWidth = m_prefilterMapSize * std::pow( 0.5, mip );
		unsigned int mipHeight = m_prefilterMapSize * std::pow( 0.5, mip );

		glBindRenderbuffer( GL_RENDERBUFFER, m_captureRBO );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight );

		glViewport( 0, 0, mipWidth, mipHeight );

		float roughness = (float)mip / (float)( maxMipLevels - 1 );
		m_prefilterShader->setFloatUniform( "_Roughness", roughness );

		for ( uint32_t i = 0; i < 6; ++i )
		{
			Matrix viewProjectionMatrix = m_captureProjection * m_captureViews[i];
			m_irradianceShader->setMatrixUniform( "_ViewProjectionMatrix", viewProjectionMatrix.transposed() );

			glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilterMap, mip );

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			static const uint8_t vertexCount = 36;
			glBindVertexArray( m_cubeVertexArray );
			glDrawArrays( GL_TRIANGLES, 0, vertexCount );
			glBindVertexArray( 0 );
		}
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void IBLMapGen::genBrdfLUTMap()
{
	m_brdfShader->bind();
	bindQuadVertexDataToShader( m_brdfShader );

	glBindFramebuffer( GL_FRAMEBUFFER, m_captureFBO );
	glBindRenderbuffer( GL_RENDERBUFFER, m_captureRBO );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_brdfLUTTextureSize, m_brdfLUTTextureSize );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brdfLUTTexture, 0 );

	glViewport( 0, 0, m_brdfLUTTextureSize, m_brdfLUTTextureSize );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static const uint8_t vertexCount = 4;
	glBindVertexArray( m_quadVertexArray );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, vertexCount );
	glBindVertexArray( 0 );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

