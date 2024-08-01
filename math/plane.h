// Oleg Kotov

#pragma once

#include "okobase.h"
#include "ray.h"

struct Plane
{
	Plane( const Vector& point, const Vector& normal )
	{
		m_normal = normal.normalized();
		m_distance = 0.0f - Vector::dotProduct( m_normal, point );
	}

	bool raycast( const Ray& ray, float& enter ) const
	{
		float nd = Vector::dotProduct( ray.m_direction, m_normal );
		float pn = Vector::dotProduct( ray.m_origin, m_normal );

		if ( nd >= 0.0f )
		{
			return false;
		}

		float t = ( m_distance - pn ) / nd;

		if ( t >= 0.0f )
		{
			enter = t;
			return true;
		}

		return false;
	}

	float m_distance;
	Vector m_normal;
};

