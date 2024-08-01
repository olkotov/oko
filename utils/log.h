// Oleg Kotov

#pragma once

#include <stdio.h>

#define Msg( format, ... ) printf( "> info: " format "\n", ##__VA_ARGS__ )
#define Warning( format, ... ) printf( "> warn: " format "\n", ##__VA_ARGS__ )

