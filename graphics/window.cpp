// Oleg Kotov

#include "window.h"
#include "log.h"

#include <SDL2/SDL.h>

Window::Window( uint16_t width, uint16_t height, const std::string& title /* = "Oko" */ )
{
	Msg( "window:initialize" );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 2 );
    
    m_window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL );

	SDL_GLContext context = SDL_GL_CreateContext( m_window );

    if ( context == NULL )
    {
		Warning( "SDL_GL_CreateContext() failed, aborting with error: %s", SDL_GetError() );
    }

    // glViewport( 0, 0, width, height );
}

Window::~Window()
{
    Msg( "window:shutdown" );
    
    SDL_GLContext currentContext = SDL_GL_GetCurrentContext();
    SDL_GL_DeleteContext( currentContext );
    SDL_DestroyWindow( m_window );
}

void Window::onResized( uint16_t width, uint16_t height ) const
{

}

void Window::getResolution( uint16_t& width, uint16_t& height ) const
{
    int32_t windowWidth = 0;
    int32_t windowHeight = 0;

	SDL_GetWindowSize( m_window, &windowWidth, &windowHeight );

    width = (uint16_t)windowWidth;
    height = (uint16_t)windowHeight;
}

float Window::getAspectRatio() const
{
    uint16_t width = 0;
    uint16_t height = 0;

    getResolution( width, height );

    return (float)width / (float)height;
}

uint16_t Window::getWidth() const
{
    int32_t width = 0;
	SDL_GetWindowSize( m_window, &width, nullptr );

	return width;
}

uint16_t Window::getHeight() const
{
    int32_t height = 0;
	SDL_GetWindowSize( m_window, nullptr, &height );

	return height;
}

void Window::setFullscreen( bool state )
{
    m_fullscreen = state;
    SDL_SetWindowFullscreen( m_window, state ? SDL_WINDOW_FULLSCREEN : 0 );
}

bool Window::isFullscreen() const
{
    return m_fullscreen;
}

void Window::setVSyncEnabled( bool enabled )
{
    m_VSyncEnabled = enabled;
    SDL_GL_SetSwapInterval( enabled ? SDL_TRUE : SDL_FALSE );
}

bool Window::isVSyncEnabled() const
{
    return m_VSyncEnabled;
}

void Window::swapBuffers() const
{
    SDL_GL_SwapWindow( m_window );
}

