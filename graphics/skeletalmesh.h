// Oleg Kotov

#pragma once

#include "okobase.h"

class Surface;

class SkeletalMesh
{    
public:

	void addSurface( Surface* surface );
	Surface* getSurface( int32_t index ) const;
	const std::vector<Surface*>& getSurfaces() const;

	void flushMesh();

private:

	std::vector<Surface*> m_surfaces;
};

