// Oleg Kotov

#include "surface.h"
#include "vertex.h"
#include "triangle.h"
#include "shader.h"

#include "okoengine.h"
#include "resourcemanager.h"

#include <GL/glew.h>

// VERTEX_ATTRIB_POSITION
// vertex attribute locations
#define VERTEX_POSITION     0
#define VERTEX_NORMAL       1
#define VERTEX_TANGENT      2
#define VERTEX_TEXCOORD     3
#define VERTEX_BLENDINDICES 4
#define VERTEX_BLENDWEIGHT  5

#define JOINTS_PER_VERTEX 4

Surface::Surface()
{
    glGenVertexArrays( 1, &m_vertexArray );
    glBindVertexArray( m_vertexArray );
    
    glGenBuffers( 1, &m_vertexBuffer );
    glGenBuffers( 1, &m_indexBuffer );
}

Surface::~Surface()
{
    glDeleteBuffers( 1, &m_indexBuffer );
    glDeleteBuffers( 1, &m_vertexBuffer );
    
    glDeleteVertexArrays( 1, &m_vertexArray );
    glBindVertexArray( 0 );
}

void Surface::setName( const std::string& name )
{
    m_name = name;
}

const std::string& Surface::getName() const
{
    return m_name;
}

void Surface::setMaterial( Material* material )
{
    m_material = material;
}

void Surface::setMaterial( const std::string& filename )
{
	Material* material = engine()->resources()->getMaterial( filename );
	assert( material );

	setMaterial( material );
}

Material* Surface::getMaterial() const
{
	return m_material;
}

void Surface::addVertex( const Vertex& vertex )
{
    m_vertices.push_back( vertex );
}

const std::vector<Vertex>& Surface::getVertices() const
{
    return m_vertices;
}

int32_t Surface::getNumVertices() const
{
    return m_numVertices;
}

void Surface::addTriangle( const Triangle& triangle )
{
	m_triangles.push_back( triangle );
}

const std::vector<Triangle>& Surface::getTriangles() const
{
    return m_triangles;
}

int32_t Surface::getNumTriangles() const
{
    return m_numTriangles;
}

int32_t Surface::getNumIndices() const
{
    return m_numIndices;
}

uint32_t Surface::getVertexArray() const
{
    return m_vertexArray;
}

void Surface::flushSurface()
{
    glBindVertexArray( m_vertexArray );

	// vertices

	m_numVertices = m_vertices.size();
	if ( m_numVertices == 0 ) return;

    glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, m_numVertices * sizeof( Vertex ), &m_vertices[0], GL_STATIC_DRAW );

    // m_vertices.Clear();
	// m_vertices.Shrink();

	// triangles

	m_numTriangles = m_triangles.size();
	if ( m_numTriangles == 0 ) return;

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_numTriangles * sizeof( Triangle ), &m_triangles[0], GL_STATIC_DRAW );

    m_numIndices = m_numTriangles * 3;

    // m_triangles.Clear();
	// m_triangles.Shrink();

    glBindVertexArray( 0 );
}

void Surface::bindToShader( Shader* shader ) const
{
    glBindVertexArray( m_vertexArray );
    glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );

    GLint positionAttrib = glGetAttribLocation( shader->getNativeHandle(), "position" );
    glEnableVertexAttribArray( positionAttrib );
    glVertexAttribPointer(
        /* index     = */ positionAttrib,
        /* size      = */ 3,
        /* type      = */ GL_FLOAT,
        /* normalize = */ GL_FALSE,
        /* stride    = */ sizeof( Vertex ),
        /* offset    = */ (GLvoid*)offsetof( Vertex, position ) );
    
    GLint normalAttrib = glGetAttribLocation( shader->getNativeHandle(), "normal" );
    glEnableVertexAttribArray( normalAttrib );
    glVertexAttribPointer(
        /* index     = */ normalAttrib,
        /* size      = */ 3,
        /* type      = */ GL_FLOAT,
        /* normalize = */ GL_FALSE,
        /* stride    = */ sizeof( Vertex ),
        /* offset    = */ (GLvoid*)offsetof( Vertex, normal ) );
    
    GLint tangentAttrib = glGetAttribLocation( shader->getNativeHandle(), "tangent" );
    glEnableVertexAttribArray( tangentAttrib );
    glVertexAttribPointer(
        /* index     = */ tangentAttrib,
        /* size      = */ 3,
        /* type      = */ GL_FLOAT,
        /* normalize = */ GL_FALSE,
        /* stride    = */ sizeof( Vertex ),
        /* offset    = */ (GLvoid*)offsetof( Vertex, tangent ) );

    GLint uvAttrib = glGetAttribLocation( shader->getNativeHandle(), "uv" );
    glEnableVertexAttribArray( uvAttrib );
    glVertexAttribPointer(
        /* index     = */ uvAttrib,
        /* size      = */ 2,
        /* type      = */ GL_FLOAT,
        /* normalize = */ GL_FALSE,
        /* stride    = */ sizeof( Vertex ),
        /* offset    = */ (GLvoid*)offsetof( Vertex, uv ) );
    
    GLint jointIndexAttrib = glGetAttribLocation( shader->getNativeHandle(), "jointIndex" );
    glEnableVertexAttribArray( jointIndexAttrib );
	glVertexAttribIPointer(
		/* index     = */ jointIndexAttrib,
		/* size      = */ JOINTS_PER_VERTEX,
		/* type      = */ GL_UNSIGNED_INT,
		/* stride    = */ sizeof( Vertex ),
		/* offset    = */ (GLvoid*)offsetof( Vertex, jointIndex ) );

    GLint jointWeightAttrib = glGetAttribLocation( shader->getNativeHandle(), "jointWeight" );
    glEnableVertexAttribArray( jointWeightAttrib );
	glVertexAttribPointer(
		/* index     = */ jointWeightAttrib,
		/* size      = */ JOINTS_PER_VERTEX,
		/* type      = */ GL_FLOAT,
		/* normalize = */ GL_FALSE,
		/* stride    = */ sizeof( Vertex ),
		/* offset    = */ (GLvoid*)offsetof( Vertex, jointWeight ) );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

