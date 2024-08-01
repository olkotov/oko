// Oleg Kotov

#include "vector.h"
#include "mathutils.h"

const Vector Vector::zero ( 0.0f, 0.0f, 0.0f );
const Vector Vector::one  ( 1.0f, 1.0f, 1.0f );

const Vector Vector::up      (  0.0f,  0.0f,  1.0f );
const Vector Vector::down    (  0.0f,  0.0f, -1.0f );
const Vector Vector::forward (  0.0f,  1.0f,  0.0f );
const Vector Vector::back    (  0.0f, -1.0f,  0.0f );
const Vector Vector::left    ( -1.0f,  0.0f,  0.0f );
const Vector Vector::right   (  1.0f,  0.0f,  0.0f );

Vector::Vector( float x, float y, float z )
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector Vector::operator-()
{
    return Vector( -x, -y, -z );
}

Vector Vector::operator+( const Vector& vec ) const
{
    Vector result;
    
    result.x = x + vec.x;
    result.y = y + vec.y;
    result.z = z + vec.z;
    
    return result;
}

Vector Vector::operator+=( const Vector& other )
{
    *this = *this + other;
    return *this;
}

Vector Vector::operator-( const Vector& vec ) const
{
    Vector result;

    result.x = x - vec.x;
    result.y = y - vec.y;
    result.z = z - vec.z;

    return result;
}

Vector Vector::operator-=( const Vector& other )
{
    *this = *this - other;
    return *this;
}

Vector Vector::operator*( const float scale ) const
{
    return Vector( x * scale, y * scale, z * scale );
}

Vector Vector::operator*=( const float scale )
{
    *this = *this * scale;
    return *this;
}

Vector Vector::operator/( const float scale ) const
{
    return Vector( x / scale, y / scale, z / scale );
}

Vector Vector::operator/=( const float scale )
{
    *this = *this / scale;
    return *this;
}

Vector Vector::operator-( const float f ) const
{
    return Vector( x - f, y - f, z - f );
}

Vector Vector::operator-=( const float f )
{
    *this = *this - f;
    return *this;
}

float Vector::length() const
{
    return sqrtf( x * x + y * y + z * z );
}

float Vector::lengthSquared() const
{
    return x * x + y * y + z * z;
}

Vector Vector::normalized() const
{
    Vector result;

    float lengthSq = lengthSquared();

    if ( lengthSq > 0.0f )
    {
        float invLength = 1.0f / sqrtf( lengthSq );
        
        result.x = x * invLength;
        result.y = y * invLength;
        result.z = z * invLength;
    }

    return result;
}

void Vector::normalize()
{
    float lengthSq = lengthSquared();

    if ( lengthSq > 0.0f )
    {
        float invLength = 1.0f / sqrtf( lengthSq );
        
        x *= invLength;
        y *= invLength;
        z *= invLength;
    }
}

Vector Vector::getClampedToMaxLength( float maxLength ) const
{
    if ( maxLength <= 0.0f )
    {
        return Vector::zero;
    }

    float lengthSq = lengthSquared();

    if ( lengthSq > maxLength * maxLength )
    {
        return normalized() * maxLength;
    }
    else
    {
        return Vector( *this );
    }
}

float Vector::dotProduct( const Vector& a, const Vector& b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector Vector::crossProduct( const Vector& a, const Vector& b )
{
    Vector result;

    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;

    return result;
}

Vector Vector::lerp( const Vector& start, const Vector& end, float amount )
{
    Vector result;
    
    result.x = start.x + ( end.x - start.x ) * amount;
    result.y = start.y + ( end.y - start.y ) * amount;
    result.z = start.z + ( end.z - start.z ) * amount;

    return result;
}

Vector Vector::moveTowards( const Vector& currentPos, const Vector& targetPos, float maxDistanceDelta )
{
    Vector dirToTarget = targetPos - currentPos;

    float distSq = dirToTarget.lengthSquared();

    if ( distSq == 0.0f || ( maxDistanceDelta >= 0.0f && distSq <= maxDistanceDelta * maxDistanceDelta ) )
    {
        return targetPos;
    }
    
    float dist = sqrtf( distSq );
    
    return currentPos + dirToTarget / dist * maxDistanceDelta;
}

void Vector::angToDir( const Vector& angles, Vector& forward )
{
    float sp, sy;
    float cp, cy;
    
    sincos( angles.x * Deg2Rad, sp, cp );
    sincos( ( angles.z + 90.0f ) * Deg2Rad, sy, cy );
    
    forward.x = cp * cy;
    forward.y = cp * sy;
    forward.z = sp;
}

// don't work
void Vector::angToDir( const Vector& angles, Vector* forward, Vector* right, Vector* up )
{
    float sp, sy, sr;
    float cp, cy, cr;
    
    sincos( angles.x * Deg2Rad, sp, cp );
    sincos( angles.y * Deg2Rad, sr, cr );
    sincos( ( angles.z + 90.0f ) * Deg2Rad, sy, cy );
    
    if ( forward != nullptr )
    {
        forward->x = cp * cy;
        forward->y = cp * sy;
        forward->z = sp;
    }
    
    if ( right != nullptr )
    {
        right->x = -1.0f * sr * sp * cy + -1.0f * cr * -sy;
        right->y = -1.0f * sr * sp * sy + -1.0f * cr *  cy;
        right->z = -1.0f * sr * cp;
    }
    
    if ( up != nullptr )
    {
        up->x = cr * sp * cy + -sr * -sy;
        up->y = cr * sp * sy + -sr *  cy;
        up->z = cr * cp;
    }
}

// not checked
Vector Vector::dirToAng( const Vector& forward )
{
    Vector angles;
    
    float pitch;
    float yaw;
    
    if ( forward.x == 0.0f && forward.y == 0.0f )
    {
        yaw = 0.0f;
        
        if ( forward.z > 0.0f )
        {
            pitch = 270.0f;
        }
        else
        {
            pitch = 90.0f;
        }
    }
    else
    {
        yaw = atan2f( forward.y, forward.x ) * Rad2Deg;
        
        if ( yaw < 0.0f )
        {
            yaw += 360.0f;
        }
        
        float temp = sqrtf( forward.x * forward.x + forward.y * forward.y );
        pitch = atan2f( -forward.z, temp ) * Rad2Deg;
        
        if ( pitch < 0.0f )
        {
            pitch += 360.0f;
        }
    }
    
    angles.x = pitch;
    angles.y = 0.0f;
    angles.z = yaw - 90.0f;
    
    return angles;
}

float Vector::pointPlaneDist( const Vector& point, const Vector& planeBase, const Vector& planeNormal )
{
    return dotProduct( ( point - planeBase ), planeNormal );
}

Vector Vector::pointPlaneProject( const Vector& point, const Vector& planeBase, const Vector& planeNormal )
{
    return point - planeNormal * pointPlaneDist( point, planeBase, planeNormal );
}

Vector Vector::project( const Vector& vector, const Vector& onNormal )
{
    float lengthSq = onNormal.lengthSquared();

    if ( lengthSq < 0.0f ) return Vector::zero;

    float dot = dotProduct( vector, onNormal );
    return onNormal * dot / lengthSq;
}

Vector Vector::projectOnPlane( const Vector& vector, const Vector& planeNormal )
{
    return vector - project( vector, planeNormal );
}

