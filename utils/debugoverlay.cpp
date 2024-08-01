// Oleg Kotov

#include "debugoverlay.h"
#include "staticmesh.h"
#include "vertex.h"
#include "cameracomponent.h"
#include "math.h"
#include "resourcemanager.h"
#include "graphics.h"
#include "shader.h"
#include "okoengine.h"
#include "log.h"
#include "skeleton.h"

#include <gl/glew.h>

DebugOverlay::DebugOverlay()
{
    Msg( "debugoverlay:initialize" );

    glGenVertexArrays( 1, &m_vertexArray );
	glGenBuffers( 1, &m_vertexBuffer );

    m_debugShader = engine()->resources()->getShader( "shaders/debug_overlay" );

    flushVertexData();
}

DebugOverlay::~DebugOverlay()
{
    Msg( "debugoverlay:shutdown" );

    glDeleteBuffers( 1, &m_vertexBuffer );
	glDeleteVertexArrays( 1, &m_vertexArray );
}

void DebugOverlay::setDrawEnabled( bool state )
{
    m_enabled = state;
}

bool DebugOverlay::isDrawEnabled() const
{
    return m_enabled;
}

void DebugOverlay::addLine( const Vector& start, const Vector& end, const Color& color /* = Color::White */ )
{
    if ( !m_enabled ) return;

    // if we have life cycle
    // m_lines.emplace_back( start, end, color );

    // add vertices?
    m_vertices.emplace_back( start, color );
    m_vertices.emplace_back( end, color );
}

void DebugOverlay::addBox( const Matrix& localToWorld, const Color& color /* = Color::White */ )
{
    Vector a = localToWorld.transformPoint( Vector( -0.5f,  0.5f, -0.5 ) );
    Vector b = localToWorld.transformPoint( Vector(  0.5f,  0.5f, -0.5 ) );
    Vector c = localToWorld.transformPoint( Vector( -0.5f,  0.5f,  0.5 ) );
    Vector d = localToWorld.transformPoint( Vector(  0.5f,  0.5f,  0.5 ) );
    Vector e = localToWorld.transformPoint( Vector( -0.5f, -0.5f, -0.5 ) );
    Vector f = localToWorld.transformPoint( Vector(  0.5f, -0.5f, -0.5 ) );
    Vector g = localToWorld.transformPoint( Vector( -0.5f, -0.5f,  0.5 ) );
    Vector h = localToWorld.transformPoint( Vector(  0.5f, -0.5f,  0.5 ) );
    
    // far plane
    addLine( a, b, color );
    addLine( b, d, color );
    addLine( d, c, color );
    addLine( c, a, color );
    
    // near plane
    addLine( e, f, color );
    addLine( f, h, color );
    addLine( h, g, color );
    addLine( g, e, color );
    
    // sides
    addLine( a, e, color );
    addLine( b, f, color );
    addLine( c, g, color );
    addLine( d, h, color );
}

void DebugOverlay::addCircle( const Vector& centerPosition, const Vector& planeNormal, float radius, const Color& color /* = Color::White */ )
{
    // TODO:
}

void DebugOverlay::addSphere( const Vector& centerPosition, float radius, const Color& color /* = Color::White */ )
{
    // TODO:
}

// https://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
void DebugOverlay::addCameraFrustum( CameraComponent* camera, const Color& color /* = Color::White */ )
{
    std::vector<Vector> points( 8 );
        
    float fov = camera->getFov();
    float aspect = camera->getAspectRatio();
    float near = camera->getNearClip();
    float far = camera->getFarClip();
    
    // camera->updateMatrices();
    // Matrix viewMatrix = camera->getViewMatrix().inverse();
    // Vector cameraPosition = viewMatrix.getTranslation();
    // Vector forward = viewMatrix.getAxisY();
    // Vector up = viewMatrix.getAxisZ();
    // Vector right = viewMatrix.getAxisX();

    Vector cameraPosition = camera->getWorldPosition();
    Vector forward = camera->getForwardDirection();
    Vector up = camera->getUpDirection();
    Vector right = camera->getRightDirection();
    
    float nearPlaneHeight = 2.0f * tanf( fov * 0.5f ) * near;
    float nearPlaneWidth = nearPlaneHeight * aspect;
    
    float farPlaneHeight = 2.0f * tanf( fov * 0.5f ) * far;
    float farPlaneWidth = farPlaneHeight * aspect;
    
    Vector farPlaneCenter = cameraPosition + forward * far;
    
    points[0] = farPlaneCenter + ( up * farPlaneHeight * 0.5f ) - ( right * farPlaneWidth * 0.5f );
    points[1] = farPlaneCenter + ( up * farPlaneHeight * 0.5f ) + ( right * farPlaneWidth * 0.5f );
    points[2] = farPlaneCenter - ( up * farPlaneHeight * 0.5f ) - ( right * farPlaneWidth * 0.5f );
    points[3] = farPlaneCenter - ( up * farPlaneHeight * 0.5f ) + ( right * farPlaneWidth * 0.5f );

    Vector nearPlaneCenter = cameraPosition + forward * near;

    points[4] = nearPlaneCenter + ( up * nearPlaneHeight * 0.5f ) - ( right * nearPlaneWidth * 0.5f );
    points[5] = nearPlaneCenter + ( up * nearPlaneHeight * 0.5f ) + ( right * nearPlaneWidth * 0.5f );
    points[6] = nearPlaneCenter - ( up * nearPlaneHeight * 0.5f ) - ( right * nearPlaneWidth * 0.5f );
    points[7] = nearPlaneCenter - ( up * nearPlaneHeight * 0.5f ) + ( right * nearPlaneWidth * 0.5f );
    
    // ------------------------------------------------
    
    // near
    
    addLine( points[0], points[1], color );
    addLine( points[1], points[3], color );
    addLine( points[3], points[2], color );
    addLine( points[2], points[0], color );
    
    // far
    
    addLine( points[4], points[5], color );
    addLine( points[5], points[7], color );
    addLine( points[7], points[6], color );
    addLine( points[6], points[4], color );
    
    // sides
    
    addLine( points[0], points[4], color );
    addLine( points[1], points[5], color );
    addLine( points[2], points[6], color );
    addLine( points[3], points[7], color );
}

void DebugOverlay::addSkeleton( Skeleton* skeleton, const Matrix& localToWorld /* = Matrix::Identity */ )
{
    addSkeletalBones( skeleton->m_root, localToWorld );
}

void DebugOverlay::addSkeletalBones( SkeletalJoint* joint, const Matrix& localToWorld )
{
	Vector ourPos = localToWorld.transformPoint( joint->m_worldMatrix.getTranslation() );

	for ( const auto& child : joint->m_children )
	{
		Vector childPos = localToWorld.transformPoint( child->m_worldMatrix.getTranslation() );
		addLine( ourPos, childPos );
        addSkeletalBones( child, localToWorld );
	}
}

void DebugOverlay::addTransformAxis( const Matrix& transform )
{
    Vector start = transform.getTranslation();

    addLine( start, start + transform.getAxisX(), Color::Red );
    addLine( start, start + transform.getAxisY(), Color::Green );
    addLine( start, start + transform.getAxisZ(), Color::Blue );
}

void DebugOverlay::flushVertexData() const
{

}

void DebugOverlay::bindVertexDataToShader( Shader* shader ) const
{
    glBindVertexArray( m_vertexArray );
    glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
    
    GLint positionAttribute = glGetAttribLocation( shader->getNativeHandle(), "position" );
	glEnableVertexAttribArray( positionAttribute );
	glVertexAttribPointer(
		/* index     = */ positionAttribute,
		/* size      = */ 3,
		/* type      = */ GL_FLOAT,
		/* normalize = */ GL_FALSE,
		/* stride    = */ sizeof( DebugVertex ),
		/* offset    = */ (void*)offsetof( DebugVertex, position ) );

    GLint colorAttribute = glGetAttribLocation( shader->getNativeHandle(), "color" );
	glEnableVertexAttribArray( colorAttribute );
	glVertexAttribPointer(
		/* index     = */ colorAttribute,
		/* size      = */ 3,
		/* type      = */ GL_FLOAT,
		/* normalize = */ GL_FALSE,
		/* stride    = */ sizeof( DebugVertex ),
		/* offset    = */ (void*)offsetof( DebugVertex, color) );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void DebugOverlay::prepareData()
{
    /*for ( const auto& line : m_lines )
    {
        m_vertices.emplace_back( line.m_start, line.m_color );
        m_vertices.emplace_back( line.m_end, line.m_color );
    }*/

    int32_t numVertices = m_vertices.size();
    if ( numVertices == 0 ) return;

    glBindVertexArray( m_vertexArray );
	glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );

	//glEnableVertexAttribArray( 0 );
	//glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( DebugVertex ), (void*)offsetof( DebugVertex, position ) );

	//glEnableVertexAttribArray( 1 );
	//glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( DebugVertex ), (void*)offsetof( DebugVertex, color ) );

	int32_t vertexBufferSize = numVertices * sizeof( DebugVertex );

	if ( m_vertexBufferAllocSize < vertexBufferSize )
	{
		m_vertexBufferAllocSize = vertexBufferSize;
		glBufferData( GL_ARRAY_BUFFER, m_vertexBufferAllocSize, NULL, GL_STREAM_DRAW );
	}

	glBufferSubData( GL_ARRAY_BUFFER, 0, vertexBufferSize, &m_vertices[0] );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void DebugOverlay::cleanup()
{
    // update life cycle

    m_vertices.clear();
    m_lines.clear();
}

void DebugOverlay::draw()
{
    if ( !m_enabled || !m_debugShader ) return;

    int32_t numVertices = m_vertices.size();

	if ( numVertices == 0 ) return;

    prepareData();

    // ---

    m_debugShader->bind();
    bindVertexDataToShader( m_debugShader );

    // setup matrices

    CameraComponent* camera = engine()->graphics()->getActiveCamera();

    const Matrix& viewProjectionMatrix = camera->getViewProjectionMatrix();

    m_debugShader->setMatrixUniform( "_ViewProjectionMatrix", viewProjectionMatrix.transposed() );

    // draw

    glDisable( GL_DEPTH_TEST );

    glBindVertexArray( m_vertexArray );
	glDrawArrays( GL_LINES, 0, numVertices );
	glBindVertexArray( 0 );

    glEnable( GL_DEPTH_TEST );

    m_debugShader->unbind();

    cleanup();
}

