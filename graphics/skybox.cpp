// Oleg Kotov

#include "skybox.h"
#include "shader.h"
#include "texture.h"
#include "okoengine.h"
#include "resourcemanager.h"
#include "graphics.h"
#include "cameracomponent.h"
#include "transformmatrix.h"
#include "log.h"

#include "ibl_map_gen.h"

#include <GL/glew.h>

Skybox::Skybox() :
	m_cubeVertices
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
{
    Msg( "skybox:initialize" );

	glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );

	glGenVertexArrays( 1, &m_vertexArray );
    glGenBuffers( 1, &m_vertexBuffer );

	flushVertexData();

	m_skyboxShader = engine()->resources()->getShader( "shaders/skybox" );

	m_axisConversionMatrix = Matrix::makeRotationAxis( Vector( 1.0f, 0.0f, 0.0f ), 0.0f );
}

Skybox::~Skybox()
{
    Msg( "skybox:shutdown" );

    glDeleteBuffers( 1, &m_vertexBuffer );
    glDeleteVertexArrays( 1, &m_vertexArray );
}

void Skybox::setDrawEnabled( bool enabled )
{
    m_drawEnabled = enabled;
}

bool Skybox::isDrawEnabled() const
{
    return m_drawEnabled;
}

void Skybox::setSkyTexture( Texture* texture )
{
    m_skyboxTexture = texture;
}

void Skybox::drawSkybox()
{
    CameraComponent* camera = engine()->graphics()->getActiveCamera();

    if ( !camera || !m_drawEnabled || !m_skyboxShader || !m_skyboxTexture ) return;

	glDepthFunc( GL_LEQUAL );

    m_skyboxShader->bind();
	bindVertexDataToShader();

    // setup matrices
    Matrix viewMatrix = camera->getViewMatrix() * m_axisConversionMatrix;
	const Matrix& projectionMatrix = camera->getProjectionMatrix();

    m_skyboxShader->setMatrixUniform( "_ViewMatrix", viewMatrix.transposed() );
	m_skyboxShader->setMatrixUniform( "_ProjectionMatrix", projectionMatrix.transposed() );
	
    // setup texture

	static const int8_t textureSlot = 0;
	glActiveTexture( GL_TEXTURE0 + textureSlot );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_skyboxTexture->getTextureId() );
	m_skyboxShader->setIntegerUniform( "_EnvMap", textureSlot );

    // draw skybox

	static const uint8_t vertexCount = 36;
    glBindVertexArray( m_vertexArray );
	glDrawArrays( GL_TRIANGLES, 0, vertexCount );
	glBindVertexArray( 0 );
	
    m_skyboxShader->unbind();

	glDepthFunc( GL_LESS );
}

void Skybox::flushVertexData() const
{
	glBindVertexArray( m_vertexArray );
    
	glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( m_cubeVertices ), &m_cubeVertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void Skybox::bindVertexDataToShader() const
{
	glBindVertexArray( m_vertexArray );
    glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
    
    GLint positionAttribute = glGetAttribLocation( m_skyboxShader->getNativeHandle(), "position" );
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

