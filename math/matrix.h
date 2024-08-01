// Oleg Kotov

#pragma once

#include "baseapi.h"

#include <stdint.h>
#include <initializer_list>

class Vector;
class Quaternion;

// row major order matrix
// opengl used column-major by default
// you can use layout( row_major ) mat4 someMatrix - to change it
// or use transposed() before sending

class ENGINE_API Matrix
{    
public:
    
    Matrix();

    Matrix( std::initializer_list<float> mat );

    Matrix( std::initializer_list<std::initializer_list<float>> mat );

    Matrix( float mat[4][4] );

    Matrix& operator=( float mat[4][4] );

    float* operator[]( int8_t index );
    const float* operator[]( int8_t index ) const;
    
    Matrix operator*( const Matrix& mat ) const;
    Matrix operator*=( const Matrix& mat );
    
    Matrix operator*( const float scale ) const;
    Matrix operator*=( const float scale );

	// Returns a transposed matrix.
    Matrix transposed() const;

	// Returns an inverse of the matrix.
    Matrix inverse() const;
    
	// Transforms position from local space to world space.
    // Position, rotation and scale.
    Vector transformPoint( const Vector& point ) const;

	// Transforms vector from local space to world space.
    // Rotation and scale.
    Vector transformVector( const Vector& vector ) const;

    // Transforms direction from local space to world space.
    // Rotation only.
    Vector transformDirection( const Vector& direction ) const; // InverseTransformVectorNoScale

    // Returns x axis from the matrix.
	Vector getAxisX() const;

    // Returns y axis from the matrix.
	Vector getAxisY() const;

    // Returns z axis from the matrix.
	Vector getAxisZ() const;

    //
	Vector getTranslation() const;

    //
	Quaternion getRotation() const;

    //
	Vector getScale() const;

public:

    static Matrix makeRotationAxis( const Vector& axis, float angle );

	// The identity matrix.
	static const Matrix Identity;

    // static int matrix_multiplication_count = 0;

protected:

	// The matrix data.
	float m_data[4][4];
};

