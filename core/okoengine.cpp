// Oleg Kotov

#include "okoengine.h"
#include "okobase.h"

#include "world.h"
#include "entity.h"

#include "clock.h"
#include "soundsystem.h"
#include "inputsystem.h"
#include "resourcemanager.h"
#include "graphics.h"
#include "physicsengine.h"
#include "profiler.h"
#include "window.h"

#include <SDL2/SDL.h>

#include <nlohmann/json.hpp>

OkoEngine* OkoEngine::m_instance = nullptr;

void OkoEngine::initialize( uint16_t windowWidth, uint16_t windowHeight, const std::string& windowTitle /* = "Oko" */ )
{
    if ( !m_instance ) m_instance = this;

    SDL_Init( SDL_INIT_EVERYTHING );
    
    srand( (uint32_t)SDL_GetPerformanceCounter() );
    
    m_clock = new Clock();
    m_resourceManager = new ResourceManager();
    m_soundSystem = new SoundSystem();
    m_inputSystem = new InputSystem();
    m_graphics = new Graphics( windowWidth, windowHeight, windowTitle );
    m_physics = new Physics();

    m_profiler = new Profiler();
}

void OkoEngine::shutdown()
{
    SafeDelete( m_world );

    SafeDelete( m_profiler );
    SafeDelete( m_physics );
    SafeDelete( m_graphics );
    SafeDelete( m_resourceManager );
    SafeDelete( m_inputSystem );
    SafeDelete( m_soundSystem );
    SafeDelete( m_clock );

    SDL_Quit();
}

OkoEngine* OkoEngine::instance()
{
    return m_instance;
}

void OkoEngine::run()
{
    m_isRunning = true;

    if ( m_world ) m_world->beginPlay();
    
    while ( m_isRunning )
    {
        float deltaTime = m_clock->update();
        float unscaledDeltaTime = m_clock->getUnscaledDeltaTime();

		m_profiler->addStats( "FPS", 1.0f / unscaledDeltaTime, "" );
		m_profiler->addStats( "DeltaTime", unscaledDeltaTime * 1000, " ms" );

        if ( m_fpsLimitEnabled )
        {
            m_timeAccumulator += deltaTime; // unscaled?

            while ( m_timeAccumulator >= m_frameTimeStep )
            {
                tick( m_frameTimeStep );
                m_timeAccumulator -= m_frameTimeStep;
            }
        }
        else
        {
            tick( deltaTime );
        }
        
        /*{
            PROFILE( "ClearBuffers", "" );
            m_graphics->clearBuffers();
        }*/

        // m_profiler->drawOnScreen();

        /*{
            PROFILE( "SwapBuffers", "" );
            m_graphics->window()->swapBuffers();
        }*/
    }
    
    shutdown();
}

void OkoEngine::tick( float deltaTime )
{
    m_frameCount++;

    // deltaTime = m_frameTimeStep;

    PROFILE( "FrameTotal", " ms" );

    {
        PROFILE( "Events", " ms" );
        processEvents();
    }

    {
        PROFILE( "Input", " ms" );
        m_inputSystem->updateKeyAxis();
    }
    
    // m_animationEngine->calculateIntermediatePoses( deltaTime );

    {
        PROFILE( "Physics", " ms" );
        m_physics->update( deltaTime );
    }

    {
        PROFILE( "Gameplay", " ms" );
        m_world->tick( deltaTime );
    }

    // m_animationEngine->finalizePoseAndMatrixPalette();

    {
        PROFILE( "Render", " ms" );

        m_graphics->renderFrame();

        if ( m_isProfilerEnabled )
        {
            m_profiler->drawOnScreen();
        }

		{
			PROFILE( "SwapBuffers", " ms" );
			m_graphics->window()->swapBuffers();
		}
    }
}

void OkoEngine::processEvents()
{
	SDL_Event event;

	while ( SDL_PollEvent( &event ) )
	{
		if ( event.type == SDL_KEYDOWN && event.key.repeat == 0 )
		{
			m_inputSystem->onKeyPressed( SDLKeycodeToKey( event.key.keysym.sym ) );
		}

		else if ( event.type == SDL_KEYUP && event.key.repeat == 0 )
		{
			m_inputSystem->onKeyReleased( SDLKeycodeToKey( event.key.keysym.sym ) );
		}

		else if ( event.type == SDL_MOUSEBUTTONDOWN )
		{
			m_inputSystem->onKeyPressed( SDLMouseButtonToKey( event.button.button ) );
		}

		else if ( event.type == SDL_MOUSEBUTTONUP )
		{
			m_inputSystem->onKeyReleased( SDLMouseButtonToKey( event.button.button ) );
		}

		else if ( event.type == SDL_MOUSEWHEEL )
		{
			if ( event.wheel.y > 0 ) // scroll up
			{
				m_inputSystem->onKeyPressed( Input::Key::MouseScrollUp );
			}
			else if ( event.wheel.y < 0 ) // scroll down
			{
				m_inputSystem->onKeyPressed( Input::Key::MouseScrollDown );
			}
		}

		else if ( event.type == SDL_WINDOWEVENT )
		{
			if ( event.window.event == SDL_WINDOWEVENT_RESIZED )
			{
				int32_t width  = event.window.data1;
				int32_t height = event.window.data2;

				m_graphics->onWindowResize( width, height );

                for ( const auto entity : m_world->getEntities() )
                {
                    entity->onResolutionChanged( width, height );
                }
			}
		}

		else if ( event.type == SDL_QUIT )
		{
			quit();
		}
	}
}

void OkoEngine::quit()
{
	m_isRunning = false;
}

void OkoEngine::changeLevel( Level* level )
{
    SafeDelete( m_world );
    m_world = level;

    if ( m_isRunning )
    {
        m_world->beginPlay();
    }
}

void OkoEngine::setLevel( Level* world )
{
    assert( world );
    m_world = world;
}

Level* OkoEngine::world() const
{
    return m_world;
}

Clock* OkoEngine::gameClock() const
{
    return m_clock;
}

SoundSystem* OkoEngine::soundSystem() const
{
    return m_soundSystem;
}

InputSystem* OkoEngine::inputSystem() const
{
    return m_inputSystem;
}

ResourceManager* OkoEngine::resources() const
{
    return m_resourceManager;
}

Graphics* OkoEngine::graphics() const
{
    return m_graphics;
}

Window* OkoEngine::window() const
{
    return graphics()->window();
}

DebugOverlay* OkoEngine::debugOverlay() const
{
    return graphics()->debugOverlay();
}

UIRenderer* OkoEngine::ui() const
{
    return graphics()->uiRenderer();
}

Skybox* OkoEngine::skybox() const
{
    return graphics()->skybox();
}

Physics* OkoEngine::physics() const
{
    return m_physics;
}

Profiler* OkoEngine::profiler() const
{
    return m_profiler;
}

// move loader code to levelLoader class
void OkoEngine::loadLevel( const std::string& levelName )
{
    std::string filepath = "/levels/" + levelName + ".json";
    std::ifstream file( filepath );

    auto json = nlohmann::json::parse( file );

    /*nlohmann::json entities = json["entities"];

	for ( auto& entity : entities )
	{
		std::string classname = entity["classname"].get<std::string>();
		std::cout << "classname: " << classname;

		if ( entity.find( "position" ) != entity.end() )
		{
            nlohmann::json position = entity["position"];
			float x = position["x"].get<float>();
			float y = position["y"].get<float>();
			float z = position["z"].get<float>();
			std::cout << ", Position: (" << x << ", " << y << ", " << z << ")";
		}

        if ( entity.find( "rotation" ) != entity.end() )
		{
            nlohmann::json rotation = entity["rotation"];
			float x = rotation["x"].get<float>();
			float y = rotation["y"].get<float>();
			float z = rotation["z"].get<float>();
			std::cout << ", Rotation: (" << x << ", " << y << ", " << z << ")";
		}

		std::cout << std::endl;
	}*/
}


void OkoEngine::setProfilerEnabled( bool enable )
{
    m_isProfilerEnabled = enable;
    m_profiler->clear();
}


bool OkoEngine::isProfilerEnabled() const
{
    return m_isProfilerEnabled;
}

OkoEngine* engine()
{
    return OkoEngine::instance();
}

