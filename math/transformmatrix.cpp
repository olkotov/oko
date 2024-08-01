// Oleg Kotov

#include "transformmatrix.h"
#include "quaternion.h"

TransformMatrix::TransformMatrix( const Vector& location, const Quaternion& rotation, const Vector& scale )
{
    float x2 = rotation.x + rotation.x;
    float y2 = rotation.y + rotation.y;
    float z2 = rotation.z + rotation.z;
    float xx = rotation.x * x2;
    float xy = rotation.x * y2;
    float xz = rotation.x * z2;
    float yy = rotation.y * y2;
    float yz = rotation.y * z2;
    float zz = rotation.z * z2;
    float wx = rotation.w * x2;
    float wy = rotation.w * y2;
    float wz = rotation.w * z2;

    m_data[0][0] = ( 1.0f - ( yy + zz ) ) * scale.x;
    m_data[0][1] = ( xy - wz ) * scale.y;
    m_data[0][2] = ( xz + wy ) * scale.z;
    m_data[0][3] = location.x;
    
    m_data[1][0] = ( xy + wz ) * scale.x;
    m_data[1][1] = ( 1.0f - ( xx + zz ) ) * scale.y;
    m_data[1][2] = ( yz - wx ) * scale.z;
    m_data[1][3] = location.y;
    
    m_data[2][0] = ( xz - wy ) * scale.x;
    m_data[2][1] = ( yz + wx ) * scale.y;
    m_data[2][2] = ( 1.0f - ( xx + yy ) ) * scale.z;
    m_data[2][3] = location.z;
    
    m_data[3][0] = 0.0f;
    m_data[3][1] = 0.0f;
    m_data[3][2] = 0.0f;
    m_data[3][3] = 1.0f;
}

