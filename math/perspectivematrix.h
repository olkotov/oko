// Oleg Kotov

#pragma once

#include "matrix.h"

class ENGINE_API PerspectiveMatrix : public Matrix
{
public:
    
    PerspectiveMatrix( float vfov, float aspect, float near, float far );
};

