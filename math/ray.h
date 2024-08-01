// Oleg Kotov

#pragma once

#include "okobase.h"

struct Ray
{
	Ray( const Vector& origin, const Vector& direction )
	{
		m_origin = origin;
		m_direction = direction.normalized();
	}

	Vector getPoint( float distance ) const
	{
		return m_origin + m_direction * distance;
	}

	Vector m_origin;
	Vector m_direction;
};

/*
	// from unreal as example
	Vector rayPlaneIntersection( const Vector& rayOrigin, const Vector& rayDirection, const Vector& planeOrigin, const Vector& planeNormal )
	{
		float distance = Vector::dotProduct( planeOrigin - rayOrigin , planeNormal ) /  Vector::dotProduct( rayDirection, planeNormal );
		return rayOrigin + rayDirection * distance;
	}

*/

