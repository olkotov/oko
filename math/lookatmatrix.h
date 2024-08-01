// Oleg Kotov

#pragma once

#include "matrix.h"
#include "vector.h"

class ENGINE_API LookAtMatrix : public Matrix
{
public:
    
    LookAtMatrix( const Vector& position, const Vector& target, const Vector& worldUp = Vector::up );
};

