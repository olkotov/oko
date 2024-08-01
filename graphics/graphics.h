// Oleg Kotov

#pragma once

#include "baseapi.h"

#include <stdint.h>
#include <string>

class Window;
class WorldRenderer;
class Skybox;
class DebugOverlay;
class UIRenderer;
class InfiniteGrid;
class IBLMapGen;

class CameraComponent;

class ENGINE_API Graphics
{        
public:
    
	Graphics( uint16_t windowWidth, uint16_t windowHeight, const std::string& windowTitle = "Oko" );
	~Graphics();
    
	void onWindowResize( uint16_t width, uint16_t height ) const;

	void clearBuffers() const;
	void renderFrame();

	// window

	uint16_t getWindowWidth() const;
	uint16_t getWindowHeight() const;

	// base renderer

	void setActiveCamera( CameraComponent* camera );
	CameraComponent* getActiveCamera() const;

	// new API
	// -------

	// base

	// setMaxFps( 60.0f ); // ?

	// void getScreenResolution();
	// void getScreenAspectRatio();

	// debug

	// drawDebugBox()
	// drawDebugCross()
	// drawDebugLine()
	
	// ui

	// drawRect()
	// drawSprite()

	// skybox
	// setDrawSkybox( bool draw );

	// world3dToScreen2d
    
	Window* window() const;
	WorldRenderer* worldRenderer() const;
	Skybox* skybox() const;
	DebugOverlay* debugOverlay() const;
	UIRenderer* uiRenderer() const;
	InfiniteGrid* infiniteGrid() const;
	IBLMapGen* iblMapGen() const;

private:

	void initGraphicsLibrary() const;
	void printDebugInfo() const;

private:

	CameraComponent* m_currentCamera = nullptr;

	Window* m_window = nullptr;
	WorldRenderer* m_worldRenderer = nullptr;
	Skybox* m_skybox = nullptr;
	DebugOverlay* m_debugOverlay = nullptr;
	UIRenderer* m_uiRenderer = nullptr;
	InfiniteGrid* m_infiniteGrid = nullptr;
	IBLMapGen* m_iblMapGen = nullptr;
};

