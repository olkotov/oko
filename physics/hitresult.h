// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "vector.h"

struct ENGINE_API HitResult
{
    ///
    class Entity* entity = nullptr;

    //
    class PrimitiveComponent* component = nullptr;

    //
    Vector position;

    /// surface normal
    Vector normal;

    //
    float distance = 0.0f;
};

