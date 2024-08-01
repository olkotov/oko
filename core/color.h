// Oleg Kotov

#pragma once

#include "baseapi.h"
#include <stdint.h>

class ENGINE_API Color
{
public:

	Color() = default;
	Color( uint8_t r, uint8_t g, uint8_t b, float a = 1.0f );

public:

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;
    
public:

    // TODO: AccentColor
    static const Color Black;
    static const Color Blue;
    static const Color Cyan;
    static const Color Gray;
    static const Color Green;
    static const Color Magenta;
    static const Color Red;
    static const Color White;
    static const Color Yellow;
};

