// Oleg Kotov

#include "gameplay.h"
#include "mathutils.h"
#include "graphics.h"
#include "cameracomponent.h"
#include "entity.h"
#include "clock.h"
#include "inputsystem.h"
#include "okoengine.h"
#include "log.h"
#include "physicsengine.h"

#include <SDL2/SDL.h>
#include <SOIL2/SOIL2.h>

float getTimeSinceStartup()
{
    return engine()->gameClock()->getTimeSinceStartup();
}

float getDeltaTime()
{
    return engine()->gameClock()->getDeltaTime();
}

void setTimeScale( float timeScale )
{
    engine()->gameClock()->setTimeScale( timeScale );
}

void getCursorPosition( int32_t* cursor_x, int32_t* cursor_y )
{
    SDL_GetMouseState( cursor_x, cursor_y );
}

void getMouseDelta( int& delta_x, int& delta_y )
{
    return engine()->inputSystem()->getMouseDelta( delta_x, delta_y );
}

void setCursorGrabbing( bool state )
{
	SDL_SetRelativeMouseMode( state ? SDL_TRUE : SDL_FALSE );
}

bool isCursorGrabbed()
{
    return (bool)SDL_GetRelativeMouseMode();
}

void setCursorVisible( bool visible )
{
    SDL_ShowCursor( (int)visible );
}

void takeScreenshot()
{
    uint16_t width  = engine()->graphics()->getWindowWidth();
    uint16_t height = engine()->graphics()->getWindowHeight();

    std::string filepath = SDL_GetBasePath(); // getWorkingDir()
    filepath += "screenshot.png";

    int32_t save_result = SOIL_save_screenshot( filepath.c_str(), SOIL_SAVE_TYPE_PNG, 0, 0, width, height );

    if ( save_result == 0 )
    {
        Warning( "taking screenshot was failed" );
    }
    else
    {
        Msg( "taking screenshot successful" );
    }
}

bool canCreateEntityClass( const std::string& classname )
{
    return EntityFactoryDictionary::instance()->findFactory( classname );
}

Entity* createEntityByName( const std::string& classname )
{
    if ( canCreateEntityClass( classname ) == true )
    {
        Msg( "%s was created", classname.c_str() );
        return EntityFactoryDictionary::instance()->create( classname );
    }
    
    return nullptr;
}

// https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
bool str_replace( std::string& str, const std::string& from, const std::string& to )
{
	size_t start_pos = str.find( from );
    if ( start_pos == std::string::npos ) return false;
	str.replace( start_pos, from.length(), to );
	return true;
}

bool raycast( HitResult& hit, const Vector& start, const Vector& end )
{
	auto physicsEngine = OkoEngine::instance()->physics();
	return physicsEngine->raycast( hit, start, end );
}

bool ENGINE_API raycastMultiple( std::vector<HitResult>& hitResults, const Vector& start, const Vector& end )
{
    auto physicsEngine = OkoEngine::instance()->physics();
	return physicsEngine->raycastMultiple( hitResults, start, end );
}

