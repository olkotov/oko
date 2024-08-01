// Oleg Kotov

#include "orthomatrix.h"

OrthoMatrix::OrthoMatrix( float left, float right, float bottom, float top, float near, float far )
{
    // float width  = right - left;
    // float height = top - bottom;
    // float depth  = far - near;

    m_data[0][0] = 2.0f / ( right - left );
    m_data[1][1] = 2.0f / ( top - bottom );
    m_data[2][2] = -2.0f / ( far - near );
    
    m_data[0][3] = -( right + left ) / ( right - left );
    m_data[1][3] = -( top + bottom ) / ( top - bottom );
    m_data[2][3] = -( far + near ) / ( far - near );
}

