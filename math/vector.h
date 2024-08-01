// Oleg Kotov

#pragma once

#include "baseapi.h"

class ENGINE_API Vector
{    
public:
    
    Vector() = default;
    Vector( float x, float y, float z );
    
    Vector operator-();
    
    Vector operator+( const Vector& vec ) const;
    Vector operator+=( const Vector& vec );
    
    Vector operator-( const Vector& vec ) const;
    Vector operator-=( const Vector& vec );
    
    Vector operator*( const float scale ) const;
    Vector operator*=( const float scale );
    
    Vector operator/( const float scale ) const;
    Vector operator/=( const float scale );

    Vector operator-( const float f ) const;
    Vector operator-=( const float f );
    
public:
    
    // Returns a length of the vector.
    float length() const;

    // Returns a squared length of the vector.
    float lengthSquared() const;

    // Returns a normalized vector.
    Vector normalized() const;
    
    // Normalize vector on place.
    void normalize();

    //
    Vector getClampedToMaxLength( float maxLength ) const;

    static float  dotProduct( const Vector& a, const Vector& b );
    static Vector crossProduct( const Vector& a, const Vector& b );

    static Vector lerp( const Vector& start, const Vector& end, float amount );
    static Vector moveTowards( const Vector& current, const Vector& target, float maxDistanceDelta );

    static void   angToDir(  const Vector& angles, Vector& forward );
    static void   angToDir( const Vector& angles, Vector* forward = nullptr, Vector* right = nullptr, Vector* up = nullptr );
    static Vector dirToAng( const Vector& forward );
    
    static float pointPlaneDist( const Vector& point, const Vector& planeBase, const Vector& planeNormal );
    static Vector pointPlaneProject( const Vector& point, const Vector& planeBase, const Vector& planeNormal );

    static Vector project( const Vector& vector, const Vector& onNormal );
    static Vector projectOnPlane( const Vector& vector, const Vector& planeNormal );

public:

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

public:

	static const Vector zero;
	static const Vector one;

	static const Vector up;
	static const Vector down;
	static const Vector forward;
	static const Vector back;
	static const Vector left;
	static const Vector right;
};

