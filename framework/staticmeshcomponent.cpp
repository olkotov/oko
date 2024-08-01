// Oleg Kotov

#include "staticmeshcomponent.h"
#include "okoengine.h"
#include "graphics.h"
#include "worldrenderer.h"
#include "staticmesh.h"
#include "surface.h"
#include "resourcemanager.h"

#include "triangle.h"
#include "vertex.h"

#include <bullet/btBulletDynamicsCommon.h>

StaticMeshComponent::StaticMeshComponent()
{
	engine()->graphics()->worldRenderer()->addStaticMeshComponent( this );

	// physics
	createCollisionShape();
	createBody();
}

StaticMeshComponent::~StaticMeshComponent()
{
	engine()->graphics()->worldRenderer()->removeStaticMeshComponent( this );

	SafeDelete( m_collisionShape );
	SafeDelete( m_physicsMesh );
}

void StaticMeshComponent::setMesh( StaticMesh* mesh )
{
	if ( !mesh ) return;

    m_mesh = mesh;

	m_materialOverrides.clear();
	m_materialOverrides.resize( mesh->getNumSurfaces() );

	createCollisionShape();
	createBody();
}

void StaticMeshComponent::setMesh( const std::string& filename )
{
	StaticMesh* mesh = engine()->resources()->getMesh( filename );
	assert( mesh );

	setMesh( mesh );
}

StaticMesh* StaticMeshComponent::getMesh() const
{
    return m_mesh;
}

void StaticMeshComponent::setMaterial( Material* material, int32_t surfaceIndex /* = 0 */ )
{
	m_materialOverrides[surfaceIndex] = material;
}

void StaticMeshComponent::setMaterial( const std::string& filename, int32_t surfaceIndex /* = 0 */ )
{
	Material* material = engine()->resources()->getMaterial( filename );
	assert( material );

	setMaterial( material, surfaceIndex );
}

Material* StaticMeshComponent::getMaterial( int32_t surfaceIndex /* = 0 */ ) const
{
	// try material overrides
	if ( surfaceIndex < m_materialOverrides.size() && m_materialOverrides[surfaceIndex] )
	{
		return m_materialOverrides[surfaceIndex];
	}

	// try mesh material
	if ( m_mesh && surfaceIndex < m_mesh->getNumSurfaces() && m_mesh->getSurface( surfaceIndex )->getMaterial() )
	{
		return m_mesh->getSurface( surfaceIndex )->getMaterial();
	}

	// or just return default material
	return engine()->graphics()->worldRenderer()->getDefaultMaterial();
}

void StaticMeshComponent::createCollisionShape()
{
	if ( !m_mesh ) return;
	if ( !m_mesh->useComplexAsSimple ) return;

	// delete nullptr is ok
	SafeDelete( m_collisionShape );
	SafeDelete( m_physicsMesh );

	m_physicsMesh = new btTriangleMesh();

	for ( const auto& surface : m_mesh->getSurfaces() )
	{
		const std::vector<Vertex>& vertices = surface->getVertices();
		const std::vector<Triangle>& triangles = surface->getTriangles();

		for ( const auto& triangle : triangles )
		{
			const Vertex& v1 = vertices[triangle.m_vertexIndex[0]];
			const Vertex& v2 = vertices[triangle.m_vertexIndex[1]];
			const Vertex& v3 = vertices[triangle.m_vertexIndex[2]];

			btVector3 vertex0( v1.position[0], v1.position[1], v1.position[2] );
			btVector3 vertex1( v2.position[0], v2.position[1], v2.position[2] );
			btVector3 vertex2( v3.position[0], v3.position[1], v3.position[2] );

			m_physicsMesh->addTriangle( vertex0, vertex1, vertex2 );
		}
	}

	const bool useQuantizedAabbCompression = true;
	m_collisionShape = new btBvhTriangleMeshShape( m_physicsMesh, useQuantizedAabbCompression );
	m_collisionShape->setUserPointer( this );
}

