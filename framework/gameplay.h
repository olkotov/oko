// Oleg Kotov

#pragma once

#include "baseapi.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>

class Entity;
class Vector;
struct HitResult;

float ENGINE_API getTimeSinceStartup();
float ENGINE_API getDeltaTime();

void ENGINE_API setTimeScale( float timeScale );

void ENGINE_API getCursorPosition( int32_t* cursor_x, int32_t* cursor_y );
void ENGINE_API getMouseDelta( int& delta_x, int& delta_y );

// Vector ENGINE_API worldToScreen( const Vector& worldPosition ); // don't work?
// Vector ENGINE_API screenToWorld( const Vector& screenPosition );

void ENGINE_API setCursorGrabbing( bool state );
bool ENGINE_API isCursorGrabbed();

void ENGINE_API setCursorVisible( bool visible );

// void playSound2D( const std::string& soundName );
// void playSoundAt( const std::string& soundName, Entity* entity );
// void playSoundAtLocation( const std::string& soundName, const Vector& location );

void ENGINE_API takeScreenshot();

ENGINE_API bool canCreateEntityClass( const std::string& classname );
ENGINE_API Entity* createEntityByName( const std::string& classname );

// bool isGamePaused();

// void spawnEntityByClass( world, classname, pos, rot );

bool ENGINE_API str_replace( std::string& str, const std::string& from, const std::string& to );

bool ENGINE_API raycast( HitResult& hit, const Vector& start, const Vector& end ); // single / multiple
bool ENGINE_API raycastMultiple( std::vector<HitResult>& hitResults, const Vector& start, const Vector& end ); // single / multiple

template <class UserType>
std::set<UserType> set_diff( const std::set<UserType>& a, const std::set<UserType>& b )
{
	std::set<UserType> result;

	std::set_difference(
		a.begin(), a.end(),
		b.begin(), b.end(),
		std::inserter( result, result.begin() )
	);

	return std::move( result );
}

