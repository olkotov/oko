// Oleg Kotov

#include "infinitegrid.h"
#include "shader.h"
#include "texture.h"
#include "okoengine.h"
#include "window.h"
#include "resourcemanager.h"
#include "graphics.h"
#include "cameracomponent.h"
#include "transformmatrix.h"
#include "log.h"

#include <GL/glew.h>

InfiniteGrid::InfiniteGrid() :
	m_gridPlaneVertices
    {
    // x, y, index (unused)
	  1.0f, -1.0f, 0.0f,
	 -1.0f,  1.0f, 0.0f,
	 -1.0f, -1.0f, 0.0f,
	 -1.0f,  1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
      1.0f,  1.0f, 0.0f
    }
{
    Msg( "infinitegrid:initialize" );

    glGenVertexArrays( 1, &m_vertexArray );
    glGenBuffers( 1, &m_vertexBuffer );

    flushVertexData();

	m_gridShader = engine()->resources()->getShader( "shaders/infinite_grid" );
}

InfiniteGrid::~InfiniteGrid()
{
    Msg( "infinitegrid:shutdown" );

    glDeleteBuffers( 1, &m_vertexBuffer );
    glDeleteVertexArrays( 1, &m_vertexArray );
}

void InfiniteGrid::setDrawEnabled( bool enabled )
{
    m_drawEnabled = enabled;
}

bool InfiniteGrid::isDrawEnabled() const
{
    return m_drawEnabled;
}

void InfiniteGrid::drawGrid()
{
    CameraComponent* camera = engine()->graphics()->getActiveCamera();

    if ( !camera || !m_drawEnabled || !m_gridShader ) return;

    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    m_gridShader->bind();

    bindVertexDataToShader();

    // setup matrices

	const Matrix& viewMatrix = camera->getViewMatrix();
	const Matrix& projectionMatrix = camera->getProjectionMatrix();

    const Matrix& viewProjectionMatrix = camera->getViewProjectionMatrix();
    Matrix viewProjectionMatrixInverse = viewProjectionMatrix.inverse();

    float nearClip = camera->getNearClip();
    float farClip = camera->getFarClip();

    m_gridShader->setMatrixUniform( "_ViewMatrix", viewMatrix.transposed() );
    m_gridShader->setMatrixUniform( "_ProjectionMatrix", projectionMatrix.transposed() );

	m_gridShader->setMatrixUniform( "_ViewProjectionMatrix", viewProjectionMatrix.transposed() );
    m_gridShader->setMatrixUniform( "_ViewProjectionMatrixInverse", viewProjectionMatrixInverse.transposed() );

    m_gridShader->setFloatUniform( "_CameraNearClip", nearClip );
    m_gridShader->setFloatUniform( "_CameraFarClip", 50.0f );

    uint16_t screenWidth, screenHeight;
	engine()->window()->getResolution( screenWidth, screenHeight );

    m_gridShader->setVectorUniform( "_DisplaySize", Vector( screenWidth, screenHeight, 0.0f ) );

    glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, 5 );
    m_gridShader->setIntegerUniform( "_SomeSampler", 0 );

    // draw grid

    static const uint8_t vertexCount = 6;
    glBindVertexArray( m_vertexArray );
	glDrawArrays( GL_TRIANGLES, 0, vertexCount );
	glBindVertexArray( 0 );
	
    m_gridShader->unbind();

    glDisable( GL_BLEND );
}

void InfiniteGrid::flushVertexData() const
{
	glBindVertexArray( m_vertexArray );
    
	glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( m_gridPlaneVertices ), &m_gridPlaneVertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void InfiniteGrid::bindVertexDataToShader() const
{
    glBindVertexArray( m_vertexArray );
    glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
    
    GLint positionAttribute = glGetAttribLocation( m_gridShader->getNativeHandle(), "position" );
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

