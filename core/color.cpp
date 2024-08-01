// Oleg Kotov

#include "color.h"

const Color Color::Black   ( 0, 0, 0, 255 );
const Color Color::White   ( 255, 255, 255, 255 );
const Color Color::Cyan    ( 0, 255, 255, 255 );
const Color Color::Gray    ( 128, 128, 128, 255 );
const Color Color::Magenta ( 255, 0, 255, 255 );

// hammer2 colors
const Color Color::Red    ( 200, 30, 30 );
const Color Color::Green  ( 50, 200, 30 );
const Color Color::Blue   ( 30, 110, 200 );
const Color Color::Yellow ( 254, 182, 45 );

Color::Color( uint8_t r, uint8_t g, uint8_t b, float a /* = 1.0f */ )
{
    this->r = r / 255.0f;
    this->g = g / 255.0f;
    this->b = b / 255.0f;
    this->a = a;
}

