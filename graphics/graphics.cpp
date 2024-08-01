// Oleg Kotov

#include "graphics.h"

#include "window.h"
#include "worldrenderer.h"
#include "skybox.h"
#include "debugoverlay.h"
#include "uirenderer.h"
#include "infinitegrid.h"
#include "ibl_map_gen.h"

#include "cameracomponent.h"

#include "log.h"
#include "profiler.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

Graphics::Graphics( uint16_t windowWidth, uint16_t windowHeight, const std::string& windowTitle /* = "Oko" */ )
{
	Msg( "graphics:initialize" );

	m_window = new Window( windowWidth, windowHeight, windowTitle );

	initGraphicsLibrary();

	//m_iblMapGen = new IBLMapGen();
	//m_iblMapGen->genEnvironmentCubeMap();
	//m_iblMapGen->genIrradianceCubeMap();
	//m_iblMapGen->genPrefilterCubeMap();
	//m_iblMapGen->genBrdfLUTMap();

	m_worldRenderer = new WorldRenderer( this );
	m_skybox = new Skybox();
	m_debugOverlay = new DebugOverlay();
	m_uiRenderer = new UIRenderer();
	m_infiniteGrid = new InfiniteGrid();

	//m_worldRenderer->setIrradianceCubeMap( m_iblMapGen->getIrradianceCubeMap() );
	//m_worldRenderer->setPrefilterCubeMap( m_iblMapGen->getPrefilterCubeMap() );
	//m_worldRenderer->setBrdfLUTMap( m_iblMapGen->getBrdfLUTMap() );

	//m_skybox->setSkyTexture( m_iblMapGen->getEnvironmentCubeMap() );
}

Graphics::~Graphics()
{
	 Msg( "graphics:shutdown" );

	 SafeDelete( m_iblMapGen );

	 SafeDelete( m_infiniteGrid );
	 SafeDelete( m_uiRenderer );
	 SafeDelete( m_debugOverlay );
	 SafeDelete( m_skybox );
	 SafeDelete( m_worldRenderer );

	 SafeDelete( m_window );
}

void Graphics::initGraphicsLibrary() const
{
	glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK )
    {
		Warning( "glewInit() failed, aborting with error code: %u", glGetError() );
		exit( -1 );
    }

	// printDebugInfo();
    
	// Setting up default render state.

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    // glEnable( GL_MULTISAMPLE );
    
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

	// TODO: add alpha support
	// glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void Graphics::printDebugInfo() const
{
	SDL_DisplayMode screenResolution;
	SDL_GetDisplayMode( 0, 0, &screenResolution );

	SDL_DisplayMode drawableSize;
	SDL_GL_GetDrawableSize( m_window->getSDLWindow(), &drawableSize.w, &drawableSize.h );

	int maxVertexAttribs;
    glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs );

	int maxTextureSize;
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize );

	GLint maxTextureUnits;
	glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits );

	// ---

	Msg( "renderer: %s", glGetString( GL_RENDERER ) );
	Msg( "vendor: %s", glGetString( GL_VENDOR ) );
	Msg( "opengl version: %s", glGetString( GL_VERSION ) );
	Msg( "glsl version: %s", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	Msg( "screen resolution: %ix%i %iHz", screenResolution.w, screenResolution.h, screenResolution.refresh_rate );
	Msg( "window resolution: %ix%i", m_window->getWidth(), m_window->getHeight() );
	Msg( "drawable size: %ix%i", drawableSize.w, drawableSize.h );

	Msg( "max vertex attributes: %i", maxVertexAttribs );
	Msg( "max texture size: %i", maxTextureSize );
	Msg( "max texture units: %i", maxTextureUnits );

    GLint numExtensions = 0;
    glGetIntegerv( GL_NUM_EXTENSIONS, &numExtensions );

	Msg( "list of available extensions (%i):", numExtensions );
	
    for ( int32_t i = 0; i < numExtensions; ++i )
    {
		Msg( "%i: %s", i, glGetStringi( GL_EXTENSIONS, i ) );
    }
}

void Graphics::onWindowResize( uint16_t width, uint16_t height ) const
{
	glViewport( 0, 0, width, height );

	float aspectRatio = (float)width / (float)height;
	m_currentCamera->setAspectRatio( aspectRatio );

	/*

	const Viewport& viewport = m_currentCamera->getViewport();

	// use render width and height?

	int32 win_width = getWindowWidth();
	int32 win_height = getWindowHeight();

	int32 x = win_width * viewport.x;
	int32 y = win_height * viewport.y;
	int32 width = win_width * viewport.width;
	int32 height = win_height * viewport.height;

	glViewport( x, y, width, height );
	
	*/
}

void Graphics::clearBuffers() const
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Graphics::setActiveCamera( CameraComponent* camera )
{
	m_currentCamera = camera;
}

CameraComponent* Graphics::getActiveCamera() const
{
	return m_currentCamera;
}

uint16_t Graphics::getWindowWidth() const
{
	return m_window->getWidth();
}

uint16_t Graphics::getWindowHeight() const
{
	return m_window->getHeight();
}

void Graphics::renderFrame()
{
	{
		PROFILE( "ClearBuffers", " ms" );
		clearBuffers();
	}

	if ( m_currentCamera )
	{
		m_currentCamera->recalculateMatrices();

		{
			PROFILE( "WorldRenderer", " ms" );
			m_worldRenderer->drawWorld();
		}

		{
			PROFILE( "Skybox", " ms" );
			m_skybox->drawSkybox();
		}

		{
			PROFILE( "DebugOverlay", " ms" );
			m_debugOverlay->draw();
		}

		{
			PROFILE( "Grid", " ms" );
			m_infiniteGrid->drawGrid();
		}

		{
			PROFILE( "UI", " ms" );
			m_uiRenderer->drawUI();
		}
	}

	{
		// PROFILE( "SwapBuffers", " ms" );
		// m_window->swapBuffers();
	}
}

Window* Graphics::window() const
{
	return m_window;
}

WorldRenderer* Graphics::worldRenderer() const
{
	return m_worldRenderer;
}

Skybox* Graphics::skybox() const
{
	return m_skybox;
}

DebugOverlay* Graphics::debugOverlay() const
{
	return m_debugOverlay;
}

UIRenderer* Graphics::uiRenderer() const
{
	return m_uiRenderer;
}

InfiniteGrid* Graphics::infiniteGrid() const
{
	return m_infiniteGrid;
}

IBLMapGen* Graphics::iblMapGen() const
{
	return m_iblMapGen;
}

