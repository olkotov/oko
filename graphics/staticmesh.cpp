// Oleg Kotov

#include "staticmesh.h"
#include "surface.h"

void StaticMesh::addSurface( Surface* surface )
{
	m_surfaces.push_back( surface );
}

Surface* StaticMesh::getSurface( int32_t index ) const
{
	return m_surfaces[index];
}

const std::vector<Surface*>& StaticMesh::getSurfaces() const
{
	return m_surfaces;
}

void StaticMesh::flushMesh()
{
	for ( auto& surface : m_surfaces )
	{
		surface->flushSurface();
	}
}

