// Oleg Kotov

#include "quaternion.h"
#include "matrix.h"
#include "mathutils.h"

const Quaternion Quaternion::Identity( 0.0f, 0.0f, 0.0f, 1.0f );

Quaternion::Quaternion( float x, float y, float z, float w )
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Quaternion Quaternion::operator*( const Quaternion& quat ) const
{
    Quaternion result;

    result.x = ( x * quat.w ) + ( w * quat.x ) + ( y * quat.z ) - ( z * quat.y );
    result.y = ( y * quat.w ) + ( w * quat.y ) + ( z * quat.x ) - ( x * quat.z );
    result.z = ( z * quat.w ) + ( w * quat.z ) + ( x * quat.y ) - ( y * quat.x );
    result.w = ( w * quat.w ) - ( x * quat.x ) - ( y * quat.y ) - ( z * quat.z );

    return result;
}

Quaternion Quaternion::operator*=( const Quaternion& other )
{
    *this = *this * other;
    return *this;
}

Quaternion Quaternion::inverse() const
{
    return Quaternion( -x, -y, -z, w );
}

Vector Quaternion::getEulerAngles() const
{
    // TODO: check this (euler order)

    Vector result;

    float test = ( w * y - x * z ) * 2.0f;

    if ( test >= 0.99999f )
    {
        result.x = atan2f( x, w ) * 2.0f;
        result.y = PI * 0.5f;
        result.z = 0.0f;

        return result;
    }

    if ( test <= -0.99999f )
    {
        result.x = atan2f( x, w ) * 2.0f;
        result.y = -PI * 0.5f;
        result.z = 0.0f;

        return result;
    }

    result.x = atan2f( 2.0f * ( w * x + y * z ), 1.0f - 2.0f * ( x * x + y * y ) );
    result.y = asinf( test );
    result.z = atan2f( 2.0f * ( w * z + x * y ), 1.0f - 2.0f * ( y * y + z * z ) );

    result *= Rad2Deg;
    
    return result;
}

Quaternion Quaternion::makeFromAxisAngle( const Vector& axis, float angle )
{
    float halfAngleRadians = angle * 0.5f * Deg2Rad;
    float s = sinf( halfAngleRadians );
    float c = cosf( halfAngleRadians );
    return Quaternion( axis.x * s, axis.y * s, axis.z * s, c );
}

Quaternion Quaternion::makeFromEuler( const Vector& angles )
{
    Quaternion result;

    Vector halfAnglesRadians = angles * Deg2Rad * 0.5f;

    float cx = cosf( halfAnglesRadians.x );
    float cy = cosf( halfAnglesRadians.y );
    float cz = cosf( halfAnglesRadians.z );

    float sx = sinf( halfAnglesRadians.x );
    float sy = sinf( halfAnglesRadians.y );
    float sz = sinf( halfAnglesRadians.z );

    result.x = sx * cy * cz - cx * sy * sz;
    result.y = cx * sy * cz + sx * cy * sz;
    result.z = cx * cy * sz - sx * sy * cz;
    result.w = cx * cy * cz + sx * sy * sz;
    
    return result;
}

Quaternion Quaternion::makeFromRotationMatrix( const Matrix& mat )
{
    Quaternion res;

    float m00 = mat[0][0];
    float m01 = mat[0][1];
    float m02 = mat[0][2];
    float m10 = mat[1][0];
    float m11 = mat[1][1];
    float m12 = mat[1][2];
    float m20 = mat[2][0];
    float m21 = mat[2][1];
    float m22 = mat[2][2];
    
    // ----
    
    float trace = m00 + m11 + m22;
    float s = 0.0f;

    if ( trace > 0.0f )
    {
        s = 0.5f / sqrtf( 1.0f + trace );

        res.x = ( m21 - m12 ) * s;
        res.y = ( m02 - m20 ) * s;
        res.z = ( m10 - m01 ) * s;
        res.w = 0.25f / s;
    }
    else
    {
        if ( m00 > m11 && m00 > m22 )
        {
            s = 0.5f / sqrtf( 1.0f + m00 - m11 - m22 );

            res.x = 0.25f / s;
            res.y = ( m01 + m10 ) * s;
            res.z = ( m20 + m02 ) * s;
            res.w = ( m21 - m12 ) * s;
        }
        else if ( m11 > m22 )
        {
            s = 0.5f / sqrtf( 1.0f + m11 - m00 - m22 );

            res.x = ( m01 + m10 ) * s;
            res.y = 0.25f / s;
            res.z = ( m12 + m21 ) * s;
            res.w = ( m02 - m20 ) * s;
        }
        else
        {
            s = 0.5f / sqrtf( 1.0f + m22 - m00 - m11 );

            res.x = ( m02 + m20 ) * s;
            res.y = ( m12 + m21 ) * s;
            res.z = 0.25f / s;
            res.w = ( m10 - m01 ) * s;
        }
    }

    return res;
}

Quaternion Quaternion::lookRotation( Vector forward, const Vector& worldUp /* = Vector::Up */ )
{
    Quaternion result;
    Matrix mat;
    
    forward = forward.normalized();
    Vector right = Vector::crossProduct( forward, worldUp ).normalized();
    Vector up = Vector::crossProduct( right, forward );
    
    mat[0][0] = right.x;
    mat[0][1] = forward.x;
    mat[0][2] = up.x;
    
    mat[1][0] = right.y;
    mat[1][1] = forward.y;
    mat[1][2] = up.y;
    
    mat[2][0] = right.z;
    mat[2][1] = forward.z;
    mat[2][2] = up.z;
    
    mat[3][3] = 1.0f;

    result = Quaternion::makeFromRotationMatrix( mat );
    
    return result;
}

// ?
//float Quaternion::DotProduct( const Quaternion& a, const Quaternion& b )
//{
//    return ( a.x * b.x ) + ( a.y * b.y ) + ( a.z * b.z ) + ( a.w * b.w );
//}

// ?
//Quaternion Quaternion::lerp( const Quaternion& a, const Quaternion& b, float alpha )
//{
//    Quaternion result;
//
//    float inverse = 1.0f - alpha;
//
//     check this, look like don't work
//    
//    if ( Quaternion::DotProduct( a, b ) >= 0.0f )
//    {
//        result.x = ( inverse * a.x ) + ( alpha * b.x );
//        result.y = ( inverse * a.y ) + ( alpha * b.y );
//        result.z = ( inverse * a.z ) + ( alpha * b.z );
//        result.w = ( inverse * a.w ) + ( alpha * b.w );
//    }
//    else
//    {
//        result.x = ( inverse * a.x ) - ( alpha * b.x );
//        result.y = ( inverse * a.y ) - ( alpha * b.y );
//        result.z = ( inverse * a.z ) - ( alpha * b.z );
//        result.w = ( inverse * a.w ) - ( alpha * b.w );
//    }
//
//    return result;
//}

Quaternion Quaternion::slerp( const Quaternion& a, const Quaternion& b, float alpha )
{
    Quaternion result;
    
	Quaternion bTemp = b;

    // calculate angle between them
    float cosHalfTheta = a.w * bTemp.w + a.x * bTemp.x + a.y * bTemp.y + a.z * bTemp.z;

    if ( cosHalfTheta < 0.0f )
    {
        bTemp.w = -bTemp.w;
        bTemp.x = -bTemp.x;
        bTemp.y = -bTemp.y;
        bTemp.z = -bTemp.z;

        cosHalfTheta = -cosHalfTheta;
    }

    // if lhs == rhs or lhs == -rhs then theta == 0 and we can return lhs
    if ( fabsf( cosHalfTheta ) >= 1.0f )
    {
       result.w = a.w;
       result.x = a.x;
       result.y = a.y;
       result.z = a.z;

       return result;
    }

    // calculate temporary values
    float halfTheta = acosf( cosHalfTheta );
    float sinHalfTheta = sqrtf( 1.0f - cosHalfTheta * cosHalfTheta );

    // if theta = 180 degrees then result is not fully defined
    // we could rotate around any axis normal to qa or qb
    if ( fabsf( sinHalfTheta ) < 0.001f )
    {
       result.w = ( a.w * 0.5f + bTemp.w * 0.5f );
       result.x = ( a.x * 0.5f + bTemp.x * 0.5f );
       result.y = ( a.y * 0.5f + bTemp.y * 0.5f );
       result.z = ( a.z * 0.5f + bTemp.z * 0.5f );

       return result;
    }

    float ratioA = sinf( ( 1.0f - alpha ) * halfTheta ) / sinHalfTheta;
    float ratioB = sinf( alpha * halfTheta ) / sinHalfTheta;

    // calculate quaternion
    result.w = ( a.w * ratioA + bTemp.w * ratioB );
    result.x = ( a.x * ratioA + bTemp.x * ratioB );
    result.y = ( a.y * ratioA + bTemp.y * ratioB );
    result.z = ( a.z * ratioA + bTemp.z * ratioB );
    
    return result;
}

Quaternion Quaternion::normalized() const
{
    Quaternion result;

    float lengthSq = x * x + y * y + z * z + w * w;

    if ( lengthSq == 0.0f )
    {
        return Identity;
    }
    else
    {
        float invLength = 1.0f / sqrtf( lengthSq );

        result.x = x * invLength;
        result.y = y * invLength;
        result.z = z * invLength;
        result.w = w * invLength;
    }
    
    return result;
}

void Quaternion::normalize()
{
    float lengthSq = x * x + y * y + z * z + w * w;

    if ( lengthSq > 0.0f )
    {
        float invLength = 1.0f / sqrtf( lengthSq );

        x *= invLength;
        y *= invLength;
        z *= invLength;
        w *= invLength;
    }
}

// from playcanvas
Vector Quaternion::transformVector( const Vector& vec ) const
{
    Vector result;
    
    // calculate quat * vec
    float ix =  w * vec.x + y * vec.z - z * vec.y;
    float iy =  w * vec.y + z * vec.x - x * vec.z;
    float iz =  w * vec.z + x * vec.y - y * vec.x;
    float iw = -x * vec.x - y * vec.y - z * vec.z;

    // calculate result * inverse quat
    result.x = ix * w + iw * -x + iy * -z - iz * -y;
    result.y = iy * w + iw * -y + iz * -x - ix * -z;
    result.z = iz * w + iw * -z + ix * -y - iy * -x;

    return result;
}

