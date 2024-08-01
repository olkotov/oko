// Oleg Kotov

#pragma once

#pragma warning( push )
#pragma warning( disable : 4251 )

// engine base
#include "baseapi.h"
#include "mathutils.h"
#include "vector.h"
#include "matrix.h"
#include "quaternion.h"
#include "color.h"
#include "clock.h"

// c++ base
#include <vector>
#include <map>
#include <set>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

// utils

#define SafeDelete( data ) \
	delete data; \
	data = nullptr;

