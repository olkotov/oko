// Oleg Kotov

#include "inputsystem.h"
#include "inputcomponent.h"
#include "log.h"

#include <SDL2/SDL.h>

#include <algorithm>

const uint8_t* keyboardState = nullptr;
uint32_t mouseState;
int32_t mouseDeltaX, mouseDeltaY;

InputSystem::InputSystem()
{
    Msg( "inputsystem:initialize" );
    keyboardState = SDL_GetKeyboardState( nullptr );
}

InputSystem::~InputSystem()
{
	Msg( "inputsystem:shutdown" );
}

void InputSystem::addInputComponent( InputComponent* component )
{
    // addUnique

	for ( int32_t i = 0; i < m_inputComponents.size(); ++i )
	{
		if ( m_inputComponents[i] == component ) return;
	}

	m_inputComponents.push_back( component );
}

void InputSystem::removeInputComponent( InputComponent* component )
{
    // removeSwap

	auto it = std::find( m_inputComponents.begin(), m_inputComponents.end(), component );

	if ( it != m_inputComponents.end() )
	{
		std::iter_swap( it, m_inputComponents.end() - 1 );
		m_inputComponents.pop_back();
	}
}

void InputSystem::addActionMapping( const std::string& actionName, Input::Key key )
{
    // addUnique

    ActionKeyMapping keymap( actionName, key );

    for ( const auto& mapping : m_actionMappings )
	{
		if ( mapping == keymap )
        {
            Warning( "action key mapping duplicate detected!" );
            return;
        }
	}

    m_actionMappings.push_back( keymap );
}

void InputSystem::addAxisMapping( const std::string& axisName, Input::Key key, float value )
{
    // addUnique

    AxisKeyMapping keymap( axisName, key, value );

    for ( const auto& mapping : m_axisMappings )
	{
        if ( mapping == keymap )
        {
            Warning( "axis key mapping duplicate detected!" );
            return;
        }
	}

	m_axisMappings.push_back( keymap );
}

void InputSystem::getMouseDelta( int& deltaX, int& deltaY ) const
{
    deltaX = mouseDeltaX;
    deltaY = mouseDeltaY;
}

void InputSystem::onKeyPressed( Input::Key key )
{
    keyPressedEvent( key );

    for ( auto& actionMapping : m_actionMappings )
    {
       if ( key != actionMapping.m_key )
       {
           continue;
       }
        
       for ( auto& inputComponent : m_inputComponents )
       {
           if ( !inputComponent->isEnabled() ) continue;

           for ( auto& actionBinding : inputComponent->m_actionBindings )
           {
               actionBinding->execute( actionMapping.m_actionName, Input::EventType::Pressed );
           }
       }
    }
}

void InputSystem::onKeyReleased( Input::Key key )
{
    keyReleasedEvent( key );

    for ( auto& actionMapping : m_actionMappings )
    {
       if ( key != actionMapping.m_key )
       {
           continue;
       }
        
       for ( auto& inputComponent : m_inputComponents )
       {
           if ( !inputComponent->isEnabled() ) continue;

           for ( auto& actionBinding : inputComponent->m_actionBindings )
           {
               actionBinding->execute( actionMapping.m_actionName, Input::EventType::Released );
           }
       }
    }
}

void InputSystem::updateKeyAxis()
{
    // update keyboard axis

    for ( int32_t i = 0; i <= SDL_NUM_SCANCODES; ++i )
    {
        if ( keyboardState[i] )
        {
            for ( auto& axisMapping : m_axisMappings )
            {
               if ( SDLScancodeToKey( i ) != axisMapping.m_key )
               {
                   continue;
               }
                
               for ( auto& inputComponent : m_inputComponents )
               {
                   if ( !inputComponent->isEnabled() ) continue;

                   for ( auto& axisBinding : inputComponent->m_axisBindings )
                   {
                       axisBinding->execute( axisMapping.m_axisName, axisMapping.m_scale );
                   }
               }
            }
        }
    }

    // retrieve mouse state
    mouseState = SDL_GetRelativeMouseState( &mouseDeltaX, &mouseDeltaY );

    // mouse x-axis delta movement
    if ( mouseDeltaX != 0 )
    {
        for ( auto& axisMapping : m_axisMappings )
        {
           if ( Input::Key::MouseX != axisMapping.m_key )
           {
               continue;
           }
            
           for ( auto& inputComponent : m_inputComponents )
           {
               if ( !inputComponent->isEnabled() ) continue;

               for ( auto& axisBinding : inputComponent->m_axisBindings )
               {
                   axisBinding->execute( axisMapping.m_axisName, mouseDeltaX * axisMapping.m_scale );
               }
           }
        }
    }
    
    // mouse y-axis delta movement
    if ( mouseDeltaY != 0 )
    {
        for ( auto& axisMapping : m_axisMappings )
        {
           if ( Input::Key::MouseY != axisMapping.m_key )
           {
               continue;
           }
            
           for ( auto& inputComponent : m_inputComponents )
           {
               if ( !inputComponent->isEnabled() ) continue;

               for ( auto& axisBinding : inputComponent->m_axisBindings )
               {
                   axisBinding->execute( axisMapping.m_axisName, mouseDeltaY * axisMapping.m_scale );
               }
           }
        }
    }

    // left button is pressed
    if ( mouseState & SDL_BUTTON( SDL_BUTTON_LEFT ) )
    {
        for ( auto& axisMapping : m_axisMappings )
        {
           if ( Input::Key::LeftMouseButton != axisMapping.m_key )
           {
               continue;
           }
            
           for ( auto& inputComponent : m_inputComponents )
           {
               if ( !inputComponent->isEnabled() ) continue;

               for ( auto& axisBinding : inputComponent->m_axisBindings )
               {
                   axisBinding->execute( axisMapping.m_axisName, axisMapping.m_scale );
               }
           }
        }
    }

    // middle button is pressed
    if ( mouseState & SDL_BUTTON( SDL_BUTTON_MIDDLE ) )
    {
        for ( auto& axisMapping : m_axisMappings )
        {
           if ( Input::Key::MiddleMouseButton != axisMapping.m_key )
           {
               continue;
           }
            
           for ( auto& inputComponent : m_inputComponents )
           {
               if ( !inputComponent->isEnabled() ) continue;

               for ( auto& axisBinding : inputComponent->m_axisBindings )
               {
                   axisBinding->execute( axisMapping.m_axisName, axisMapping.m_scale );
               }
           }
        }
    }

    // right button is pressed
    if ( mouseState & SDL_BUTTON( SDL_BUTTON_RIGHT ) )
    {
        for ( auto& axisMapping : m_axisMappings )
        {
           if ( Input::Key::RightMouseButton != axisMapping.m_key )
           {
               continue;
           }
            
           for ( auto& inputComponent : m_inputComponents )
           {
               if ( !inputComponent->isEnabled() ) continue;

               for ( auto& axisBinding : inputComponent->m_axisBindings )
               {
                   axisBinding->execute( axisMapping.m_axisName, axisMapping.m_scale );
               }
           }
        }
    }
}

bool InputSystem::isKeyPressed( Input::Key key ) const
{
    if ( key == Input::Key::LeftMouseButton )
    {
        return ( mouseState & SDL_BUTTON( SDL_BUTTON_LEFT ) );
    }
}

