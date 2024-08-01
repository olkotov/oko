// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "vector.h"

class Matrix;

class ENGINE_API Quaternion
{    
public:
    
	Quaternion() = default;
    Quaternion( float x, float y, float z, float w );
    
    Quaternion operator*( const Quaternion& other ) const;
    Quaternion operator*=( const Quaternion& other );

    // Returns an inverse of the quaternion.
    Quaternion inverse() const;
    
    // Returns an euler angles of the quaternion.
    Vector getEulerAngles() const;

    static Quaternion makeFromAxisAngle( const Vector& axis, float angle );

    static Quaternion makeFromEuler( const Vector& angles );
    static Quaternion makeFromRotationMatrix( const Matrix& mat );
    
    static Quaternion lookRotation( Vector forward, const Vector& worldUp = Vector::up );
    
    // static float DotProduct( const Quaternion& a, const Quaternion& b ); // ?
    // static Quaternion lerp( const Quaternion& a, const Quaternion& b, float alpha );
    static Quaternion slerp( const Quaternion& a, const Quaternion& b, float alpha );

    // Returns normalized quaternion.
    Quaternion normalized() const;

    void normalize();
    
    Vector transformVector( const Vector& vec ) const;

public:

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;

	static const Quaternion Identity;
};

