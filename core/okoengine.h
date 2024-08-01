// Oleg Kotov

#pragma once

#include "okobase.h"

class Level;
class Clock;
class InputSystem;
class ResourceManager;
class Graphics;
class Window;
class DebugOverlay;
class UIRenderer;
class Skybox;
class Physics;
class SoundSystem;
class Profiler;

class ENGINE_API OkoEngine
{    
public:
        
    void initialize( uint16_t windowWidth, uint16_t windowHeight, const std::string& windowTitle = "Oko" );
	void shutdown();

	static OkoEngine* instance();

    void run();
    void tick( float deltaTime );
	void processEvents();
	void quit();

	// changeWorld( "name" )
	void changeLevel( Level* level );
	void setLevel( Level* world );
	Level* world() const;

	Clock* gameClock() const;
	SoundSystem* soundSystem() const;
	InputSystem* inputSystem() const;
	ResourceManager* resources() const;
	Graphics* graphics() const;
	Window* window() const;
	DebugOverlay* debugOverlay() const;
	UIRenderer* ui() const;
	Skybox* skybox() const;
	Physics* physics() const;
	Profiler* profiler() const;

	void loadLevel( const std::string& levelName );

	// void setTimeScale( float timeScale );

	void setProfilerEnabled( bool enable );
	bool isProfilerEnabled() const;

private:

	static OkoEngine* m_instance;

	// Is this game running?
	bool m_isRunning = false;

	uint64_t m_frameCount = 0;

	// The game world.
	Level* m_world = nullptr;

	Clock* m_clock = nullptr;
	SoundSystem* m_soundSystem = nullptr;
	InputSystem* m_inputSystem = nullptr;
	ResourceManager* m_resourceManager = nullptr;
	Graphics* m_graphics = nullptr;
	Physics* m_physics = nullptr;
	Profiler* m_profiler = nullptr;

	bool m_fpsLimitEnabled = false;
	float m_maxFps = 30.0f;
	float m_frameTimeStep = 1.0f / m_maxFps;
	float m_timeAccumulator = 0.0f;

	bool m_isProfilerEnabled = false;
};

ENGINE_API OkoEngine* engine();

