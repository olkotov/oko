// Oleg Kotov

#include "skeletalmesh.h"
#include "surface.h"

void SkeletalMesh::addSurface( Surface* surface )
{
	m_surfaces.push_back( surface );
}

Surface* SkeletalMesh::getSurface( int32_t index ) const
{
	return m_surfaces[index];
}

const std::vector<Surface*>& SkeletalMesh::getSurfaces() const
{
	return m_surfaces;
}

void SkeletalMesh::flushMesh()
{
	for ( auto& getSurface : m_surfaces )
	{
		getSurface->flushSurface();
	}
}

