// Oleg Kotov

#pragma once

#include "baseapi.h"

#include <stdint.h>
#include <vector>

class Surface;

class ENGINE_API StaticMesh
{
public:

	void addSurface( Surface* surface );
	Surface* getSurface( int32_t index ) const;
	const std::vector<Surface*>& getSurfaces() const;

	uint32_t getNumSurfaces() const { return m_surfaces.size(); }
	bool hasSurfaces() const { return m_surfaces.size() > 0; }

    void flushMesh();

	bool useComplexAsSimple = false;

private:

	std::vector<Surface*> m_surfaces;
};

