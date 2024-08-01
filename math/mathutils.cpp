// Oleg Kotov

#include "mathutils.h"

#define _USE_MATH_DEFINES
#include <math.h> // M_PI

#include <float.h> // FLT_EPSILON
#include <stdlib.h> // rand()

const float Epsilon = FLT_EPSILON;
const float PI = M_PI; // 3.141593f;
const float Deg2Rad = PI / 180.0f;
const float Rad2Deg = 180.0f / PI;

float lerp( float start, float end, float amount )
{
    return start + ( end - start ) * amount;
}

float clamp( int value, int min, int max )
{
    if ( value >= max ) return max;
    if ( value <= min ) return min;
    return value;
}

float clamp( float value, float min, float max )
{
    if ( value >= max ) return max;
    if ( value <= min ) return min;
    return value;
}

float sign( float f )
{
    if ( f > 0.0f ) return 1.0f;
    if ( f < 0.0f ) return -1.0f;
    return 0.0f;
}

float min( float a, float b )
{
    return ( a < b ) ? a : b;
}

float max( float a, float b )
{
    return ( a > b ) ? a : b;
}

bool getRandomBool()
{
    return rand() % 2 == 0;
}

float getRandomNumber()
{
    return rand() / (float)RAND_MAX;
}

int getRandomNumberInRange( int min, int max )
{
    return min + rand() % ( max - min + 1 );
}

float getRandomNumberInRange( float min, float max )
{
    return rand() / (float)RAND_MAX * ( max - min ) + min;
}

void getRandomUnitVector2d( float& x, float& y )
{
    float angle = getRandomNumberInRange( (float)-M_PI, (float)M_PI );
	
    // direction
    x = cosf( angle );
    y = sinf( angle );
}

bool almostEquals( float a, float b )
{
    return fabsf( a - b ) < Epsilon;
}

void sincos( float angle, float& sin, float& cos )
{
    sin = sinf( angle );
    cos = cosf( angle );
}

