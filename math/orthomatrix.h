// Oleg Kotov

#pragma once

#include "matrix.h"

class ENGINE_API OrthoMatrix : public Matrix
{
public:
    
    OrthoMatrix( float left, float right, float bottom, float top, float near, float far );
};

