// Oleg Kotov

#pragma once

#include "baseapi.h"

#include <stdint.h>
#include <string>

enum class FullscreenMode : uint8_t
{
	windowed,
	fullscreen,
	borderless
};

class ENGINE_API Window
{        
public:
    
	Window( uint16_t width, uint16_t height, const std::string& title = "Oko" );
	~Window();
    
	// void setTitle( const std::string& title );
	// std::string getTitle() const;

	void onResized( uint16_t width, uint16_t height ) const;

	void getResolution( uint16_t& width, uint16_t& height ) const;
	float getAspectRatio() const;

	// void setWidth( uint16_t width );
	uint16_t getWidth() const;

	// void setHeight( uint16_t height );
	uint16_t getHeight() const;

	void setFullscreen( bool state );
	bool isFullscreen() const;

	void setVSyncEnabled( bool enable );
	bool isVSyncEnabled() const;

	void swapBuffers() const;

	// void setMSAAEnabled( bool state );

	struct SDL_Window* getSDLWindow() const { return m_window; }

private:

	bool m_VSyncEnabled = false;
	bool m_fullscreen = false;

	struct SDL_Window* m_window = nullptr;
};

