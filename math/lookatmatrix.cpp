// Oleg Kotov

#include "lookatmatrix.h"

LookAtMatrix::LookAtMatrix( const Vector& position, const Vector& target, const Vector& worldUp /* = Vector::Up */ )
{
    Vector zaxis = ( position - target ).normalized();
    Vector xaxis = Vector::crossProduct( worldUp, zaxis ).normalized();
    Vector yaxis = Vector::crossProduct( zaxis, xaxis );

    m_data[0][0] = xaxis.x;
    m_data[1][0] = yaxis.x;
    m_data[2][0] = zaxis.x;
    
    m_data[0][1] = xaxis.y;
    m_data[1][1] = yaxis.y;
    m_data[2][1] = zaxis.y;
    
    m_data[0][2] = xaxis.z;
    m_data[1][2] = yaxis.z;
    m_data[2][2] = zaxis.z;
    
    m_data[0][3] = -Vector::dotProduct( xaxis, position );
    m_data[1][3] = -Vector::dotProduct( yaxis, position );
    m_data[2][3] = -Vector::dotProduct( zaxis, position );
}

