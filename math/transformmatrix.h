// Oleg Kotov

#pragma once

#include "matrix.h"
#include "vector.h"

class Quaternion;

class ENGINE_API TransformMatrix : public Matrix
{
public:

    TransformMatrix( const Vector& location, const Quaternion& rotation, const Vector& scale = Vector::one );
};

