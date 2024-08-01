// Oleg Kotov

#include "perspectivematrix.h"
#include <math.h>

PerspectiveMatrix::PerspectiveMatrix( float vfov, float aspect, float near, float far )
{
    float yScale = 1.0f / tanf( vfov * 0.5f );
    float xScale = yScale / aspect;

    float m22 = -( far + near ) / ( far - near );
    float m23 = near * far * 2.0f / ( near - far );

    m_data[0][0] = xScale;
    m_data[1][1] = yScale;

    m_data[2][2] = m22;
    m_data[2][3] = m23;

    m_data[3][2] = -1.0f;
    m_data[3][3] = 0.0f; // !
}

