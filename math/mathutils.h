// Oleg Kotov

#pragma once

#include "baseapi.h"
#include <math.h>

extern ENGINE_API const float Epsilon;
extern ENGINE_API const float PI;
extern ENGINE_API const float Deg2Rad;
extern ENGINE_API const float Rad2Deg;

ENGINE_API float lerp( float start, float end, float amount );
ENGINE_API float clamp( int value, int min, int max );
ENGINE_API float clamp( float value, float min, float max );
ENGINE_API float sign( float f );
// unitsign
ENGINE_API float min( float a, float b );
ENGINE_API float max( float a, float b );

bool getRandomBool();
float getRandomNumber(); // 0.0f - 1.0f
ENGINE_API int getRandomNumberInRange( int min, int max );
ENGINE_API float getRandomNumberInRange( float min, float max );
void getRandomUnitVector2d( float& x, float& y );

ENGINE_API bool almostEquals( float a, float b );

// todo: remove?
ENGINE_API void sincos( float angle, float& sin, float& cos );

