// Oleg Kotov

#include "matrix.h"
#include "vector.h"
#include "quaternion.h"
#include "mathutils.h"

#include <string.h>

const Matrix Matrix::Identity;

Matrix::Matrix()
    : m_data
    {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    }
{}

Matrix::Matrix( std::initializer_list<float> mat )
{
	auto itr = mat.begin();

	for ( int i = 0; i < 4; ++i )
	{
		for ( int j = 0; j < 4; ++j )
		{
			m_data[i][j] = *itr++;
		}
	}
}

Matrix::Matrix( std::initializer_list<std::initializer_list<float>> mat )
{
	auto row = mat.begin();

	for ( int i = 0; i < 4; ++i )
	{
		auto col = ( *row++ ).begin();
			
		for ( int j = 0; j < 4; ++j )
		{
			m_data[i][j] = *col++;
		}
	}
}

Matrix::Matrix( float mat[4][4] )
{
	memcpy( m_data, mat, sizeof( m_data ) );
}

Matrix& Matrix::operator=( float mat[4][4] )
{
	memcpy( m_data, mat, sizeof( m_data ) );
	return *this;
}

float* Matrix::operator[]( int8_t index )
{
    return m_data[index];
}

const float* Matrix::operator[]( int8_t index ) const
{
    return m_data[index];
}

Matrix Matrix::operator*( const Matrix& mat ) const
{
    Matrix result;
    
    result[0][0] = m_data[0][0] * mat[0][0] + m_data[0][1] * mat[1][0] + m_data[0][2] * mat[2][0] + m_data[0][3] * mat[3][0];
    result[0][1] = m_data[0][0] * mat[0][1] + m_data[0][1] * mat[1][1] + m_data[0][2] * mat[2][1] + m_data[0][3] * mat[3][1];
    result[0][2] = m_data[0][0] * mat[0][2] + m_data[0][1] * mat[1][2] + m_data[0][2] * mat[2][2] + m_data[0][3] * mat[3][2];
    result[0][3] = m_data[0][0] * mat[0][3] + m_data[0][1] * mat[1][3] + m_data[0][2] * mat[2][3] + m_data[0][3] * mat[3][3];

    result[1][0] = m_data[1][0] * mat[0][0] + m_data[1][1] * mat[1][0] + m_data[1][2] * mat[2][0] + m_data[1][3] * mat[3][0];
    result[1][1] = m_data[1][0] * mat[0][1] + m_data[1][1] * mat[1][1] + m_data[1][2] * mat[2][1] + m_data[1][3] * mat[3][1];
    result[1][2] = m_data[1][0] * mat[0][2] + m_data[1][1] * mat[1][2] + m_data[1][2] * mat[2][2] + m_data[1][3] * mat[3][2];
    result[1][3] = m_data[1][0] * mat[0][3] + m_data[1][1] * mat[1][3] + m_data[1][2] * mat[2][3] + m_data[1][3] * mat[3][3];
    
    result[2][0] = m_data[2][0] * mat[0][0] + m_data[2][1] * mat[1][0] + m_data[2][2] * mat[2][0] + m_data[2][3] * mat[3][0];
    result[2][1] = m_data[2][0] * mat[0][1] + m_data[2][1] * mat[1][1] + m_data[2][2] * mat[2][1] + m_data[2][3] * mat[3][1];
    result[2][2] = m_data[2][0] * mat[0][2] + m_data[2][1] * mat[1][2] + m_data[2][2] * mat[2][2] + m_data[2][3] * mat[3][2];
    result[2][3] = m_data[2][0] * mat[0][3] + m_data[2][1] * mat[1][3] + m_data[2][2] * mat[2][3] + m_data[2][3] * mat[3][3];

    result[3][0] = m_data[3][0] * mat[0][0] + m_data[3][1] * mat[1][0] + m_data[3][2] * mat[2][0] + m_data[3][3] * mat[3][0];
    result[3][1] = m_data[3][0] * mat[0][1] + m_data[3][1] * mat[1][1] + m_data[3][2] * mat[2][1] + m_data[3][3] * mat[3][1];
    result[3][2] = m_data[3][0] * mat[0][2] + m_data[3][1] * mat[1][2] + m_data[3][2] * mat[2][2] + m_data[3][3] * mat[3][2];
    result[3][3] = m_data[3][0] * mat[0][3] + m_data[3][1] * mat[1][3] + m_data[3][2] * mat[2][3] + m_data[3][3] * mat[3][3];
    
    return result;
}

Matrix Matrix::operator*=( const Matrix& mat )
{
    *this = *this * mat;
    return *this;
}

Matrix Matrix::operator*( const float scale ) const
{
    Matrix result;
    
    result[0][0] *= scale;
    result[0][1] *= scale;
    result[0][2] *= scale;
    result[0][3] *= scale;

    result[1][0] *= scale;
    result[1][1] *= scale;
    result[1][2] *= scale;
    result[1][3] *= scale;

    result[2][0] *= scale;
    result[2][1] *= scale;
    result[2][2] *= scale;
    result[2][3] *= scale;

    result[3][0] *= scale;
    result[3][1] *= scale;
    result[3][2] *= scale;
    result[3][3] *= scale;
    
    return result;
}

Matrix Matrix::operator*=( const float other )
{
    *this = *this * other;
    return *this;
}

Matrix Matrix::transposed() const
{
    Matrix result;
    
    result[0][0] = m_data[0][0];
    result[0][1] = m_data[1][0];
    result[0][2] = m_data[2][0];
    result[0][3] = m_data[3][0];
    
    result[1][0] = m_data[0][1];
    result[1][1] = m_data[1][1];
    result[1][2] = m_data[2][1];
    result[1][3] = m_data[3][1];
    
    result[2][0] = m_data[0][2];
    result[2][1] = m_data[1][2];
    result[2][2] = m_data[2][2];
    result[2][3] = m_data[3][2];
    
    result[3][0] = m_data[0][3];
    result[3][1] = m_data[1][3];
    result[3][2] = m_data[2][3];
    result[3][3] = m_data[3][3];
    
    return result;
}

Matrix Matrix::inverse() const
{
    Matrix result;
    
    float t00 = m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0];
    float t01 = m_data[0][0] * m_data[1][2] - m_data[0][2] * m_data[1][0];
    float t02 = m_data[0][0] * m_data[1][3] - m_data[0][3] * m_data[1][0];
    float t03 = m_data[0][1] * m_data[1][2] - m_data[0][2] * m_data[1][1];
    float t04 = m_data[0][1] * m_data[1][3] - m_data[0][3] * m_data[1][1];
    float t05 = m_data[0][2] * m_data[1][3] - m_data[0][3] * m_data[1][2];
    float t06 = m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0];
    float t07 = m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0];
    float t08 = m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0];
    float t09 = m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1];
    float t10 = m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1];
    float t11 = m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2];

    float det = ( t00 * t11 - t01 * t10 + t02 * t09 + t03 * t08 - t04 * t07 + t05 * t06 );
    
	// The determinant must not be zero.

    if ( det == 0.0f ) return Matrix::Identity;

    float invDet = 1.0f / det;

    result[0][0] = (  m_data[1][1] * t11 - m_data[1][2] * t10 + m_data[1][3] * t09 ) * invDet;
    result[0][1] = ( -m_data[0][1] * t11 + m_data[0][2] * t10 - m_data[0][3] * t09 ) * invDet;
    result[0][2] = (  m_data[3][1] * t05 - m_data[3][2] * t04 + m_data[3][3] * t03 ) * invDet;
    result[0][3] = ( -m_data[2][1] * t05 + m_data[2][2] * t04 - m_data[2][3] * t03 ) * invDet;
        
    result[1][0] = ( -m_data[1][0] * t11 + m_data[1][2] * t08 - m_data[1][3] * t07 ) * invDet;
    result[1][1] = (  m_data[0][0] * t11 - m_data[0][2] * t08 + m_data[0][3] * t07 ) * invDet;
    result[1][2] = ( -m_data[3][0] * t05 + m_data[3][2] * t02 - m_data[3][3] * t01 ) * invDet;
    result[1][3] = (  m_data[2][0] * t05 - m_data[2][2] * t02 + m_data[2][3] * t01 ) * invDet;
        
    result[2][0] = (  m_data[1][0] * t10 - m_data[1][1] * t08 + m_data[1][3] * t06 ) * invDet;
    result[2][1] = ( -m_data[0][0] * t10 + m_data[0][1] * t08 - m_data[0][3] * t06 ) * invDet;
    result[2][2] = (  m_data[3][0] * t04 - m_data[3][1] * t02 + m_data[3][3] * t00 ) * invDet;
    result[2][3] = ( -m_data[2][0] * t04 + m_data[2][1] * t02 - m_data[2][3] * t00 ) * invDet;
        
    result[3][0] = ( -m_data[1][0] * t09 + m_data[1][1] * t07 - m_data[1][2] * t06 ) * invDet;
    result[3][1] = (  m_data[0][0] * t09 - m_data[0][1] * t07 + m_data[0][2] * t06 ) * invDet;
    result[3][2] = ( -m_data[3][0] * t03 + m_data[3][1] * t01 - m_data[3][2] * t00 ) * invDet;
    result[3][3] = (  m_data[2][0] * t03 - m_data[2][1] * t01 + m_data[2][2] * t00 ) * invDet;
    
    return result;
}

Vector Matrix::transformPoint( const Vector& point ) const
{
    Vector result;

    result.x = m_data[0][0] * point.x + m_data[0][1] * point.y + m_data[0][2] * point.z + m_data[0][3];
    result.y = m_data[1][0] * point.x + m_data[1][1] * point.y + m_data[1][2] * point.z + m_data[1][3];
    result.z = m_data[2][0] * point.x + m_data[2][1] * point.y + m_data[2][2] * point.z + m_data[2][3];

    return result;
}

Vector Matrix::transformVector( const Vector& vector ) const
{
    Vector result;
    
    result.x = m_data[0][0] * vector.x + m_data[0][1] * vector.y + m_data[0][2] * vector.z;
    result.y = m_data[1][0] * vector.x + m_data[1][1] * vector.y + m_data[1][2] * vector.z;
    result.z = m_data[2][0] * vector.x + m_data[2][1] * vector.y + m_data[2][2] * vector.z;

    return result;
}

Vector Matrix::transformDirection( const Vector& direction ) const
{
    return getRotation().transformVector( direction );
}

Vector Matrix::getAxisX() const
{
	Vector result;

	result.x = m_data[0][0];
	result.y = m_data[1][0];
	result.z = m_data[2][0];

	return result;
}

Vector Matrix::getAxisY() const
{
	Vector result;

	result.x = m_data[0][1];
	result.y = m_data[1][1];
	result.z = m_data[2][1];

	return result;
}

Vector Matrix::getAxisZ() const
{
	Vector result;

	result.x = m_data[0][2];
	result.y = m_data[1][2];
	result.z = m_data[2][2];

	return result;
}

Vector Matrix::getTranslation() const
{
	Vector result;

	result.x = m_data[0][3];
	result.y = m_data[1][3];
	result.z = m_data[2][3];

	return result;
}

Quaternion Matrix::getRotation() const
{
	Quaternion result;

	// extract rotation matrix

	Matrix rotMat;

	Vector scale = getScale();

	scale.x = 1.0f / ( scale.x + Epsilon );
	scale.y = 1.0f / ( scale.y + Epsilon );
	scale.z = 1.0f / ( scale.z + Epsilon );

	rotMat[0][0] = m_data[0][0] * scale.x;
	rotMat[0][1] = m_data[0][1] * scale.y;
	rotMat[0][2] = m_data[0][2] * scale.z;

	rotMat[1][0] = m_data[1][0] * scale.x;
	rotMat[1][1] = m_data[1][1] * scale.y;
	rotMat[1][2] = m_data[1][2] * scale.z;

	rotMat[2][0] = m_data[2][0] * scale.x;
	rotMat[2][1] = m_data[2][1] * scale.y;
	rotMat[2][2] = m_data[2][2] * scale.z;

	// matrix to quat convert

	float trace = rotMat[0][0] + rotMat[1][1] + rotMat[2][2];
	float s = 0.0f;

	if ( trace > 0.0f )
	{
		s = 0.5f / sqrtf( 1.0f + trace );

		result.x = ( rotMat[2][1] - rotMat[1][2] ) * s;
		result.y = ( rotMat[0][2] - rotMat[2][0] ) * s;
		result.z = ( rotMat[1][0] - rotMat[0][1] ) * s;
		result.w = 0.25f / s;
	}
	else
	{
		if ( rotMat[0][0] > rotMat[1][1] && rotMat[0][0] > rotMat[2][2] )
		{
			s = 0.5f / sqrtf( 1.0f + rotMat[0][0] - rotMat[1][1] - rotMat[2][2] );

			result.x = 0.25f / s;
			result.y = ( rotMat[0][1] + rotMat[1][0] ) * s;
			result.z = ( rotMat[2][0] + rotMat[0][2] ) * s;
			result.w = ( rotMat[2][1] - rotMat[1][2] ) * s;
		}
		else if ( rotMat[1][1] > rotMat[2][2] )
		{
			s = 0.5f / sqrtf( 1.0f + rotMat[1][1] - rotMat[0][0] - rotMat[2][2] );

			result.x = ( rotMat[0][1] + rotMat[1][0] ) * s;
			result.y = 0.25f / s;
			result.z = ( rotMat[1][2] + rotMat[2][1] ) * s;
			result.w = ( rotMat[0][2] - rotMat[2][0] ) * s;
		}
		else
		{
			s = 0.5f / sqrtf( 1.0f + rotMat[2][2] - rotMat[0][0] - rotMat[1][1] );

			result.x = ( rotMat[0][2] + rotMat[2][0] ) * s;
			result.y = ( rotMat[1][2] + rotMat[2][1] ) * s;
			result.z = 0.25f / s;
			result.w = ( rotMat[1][0] - rotMat[0][1] ) * s;
		}
	}

	return result;
}

Vector Matrix::getScale() const
{
	Vector result;

	result.x = getAxisX().length();
	result.y = getAxisY().length();
	result.z = getAxisZ().length();

	return result;
}

Matrix Matrix::makeRotationAxis( const Vector& axis, float angle )
{
    Matrix mat;

    float angleRadians = angle * Deg2Rad;

    float cos = cosf( angleRadians );
	float sin = sinf( angleRadians );
	float t = 1.0f - cos;
	float tx = t * axis.x;
	float ty = t * axis.y;

    mat[0][0] = tx * axis.x + cos;
    mat[1][0] = tx * axis.y + sin * axis.z;
    mat[2][0] = tx * axis.z - sin * axis.y;

    mat[0][1] = tx * axis.y - sin * axis.z;
    mat[1][1] = ty * axis.y + cos;
    mat[2][1] = ty * axis.z + sin * axis.x;

    mat[0][2] = tx * axis.z + sin * axis.y;
    mat[1][2] = ty * axis.z - axis.x * sin;
    mat[2][2] = t * axis.z * axis.z + cos;

    return mat;
}

