// Oleg Kotov

#pragma once

#include "entitycomponent.h"
#include <stdint.h>

/**
* An InputEvent.
***********************************************************************************/

namespace Input
{
    enum class EventType : uint8_t
    {
        Pressed,
        Released
    };
};

/**
* An InputBinding interface class.
* I need this to store the template classes in the array. WTFFFFACK?
***********************************************************************************/

class ENGINE_API InputBinding
{
public:
    
	virtual ~InputBinding() {}

    virtual void execute( const std::string& actionName, Input::EventType eventType ) {}
    virtual void execute( const std::string& axisName, float value ) {}
};

/**
* An ActionBinding.
***********************************************************************************/

template <class UserType>
class ActionBinding : public InputBinding
{
public:

    ActionBinding( const std::string& actionName, Input::EventType eventType, UserType* instance, void ( UserType::*handler )() )
		: m_actionName( actionName )
		, m_eventType( eventType )
		, m_instance( instance )
		, m_handler( handler )
	{}
    
    void execute( const std::string& actionName, Input::EventType eventType )
    {
        if ( m_actionName == actionName && m_eventType == eventType )
        {
            ( m_instance->*m_handler )();
        }
    }
    
private:

    std::string m_actionName;        // Bind "Jump" action
    Input::EventType m_eventType;    // with Pressed event type
    UserType* m_instance;            // to the Entity instance
    void ( UserType::*m_handler )(); // and Jump function.
};

/**
* An AxisBinding.
***********************************************************************************/

template <class UserType>
class AxisBinding : public InputBinding
{
public:
    AxisBinding( const std::string& axisName, UserType* instance, void ( UserType::*handler )( float ) )
		: m_axisName( axisName )
		, m_instance( instance )
		, m_handler( handler ) 
	{}
    
    void execute( const std::string& axisName, float value )
    {
        if ( m_axisName == axisName )
        {
            ( m_instance->*m_handler )( value );
        }
    }
        
private:

    std::string m_axisName;                 // Bind "MoveForward" axis
    UserType* m_instance;                   // to the Entity instance
    void ( UserType::*m_handler )( float ); // and MoveForward function.
};

/**
* An InputComponent.
***********************************************************************************/

class ENGINE_API InputComponent : public EntityComponent
{
	friend class InputSystem;

public:
    
    InputComponent();
    ~InputComponent();
    
    // typedef std::function<void( int activationMode, float value )> ActionCallback;
    // registerAction()

    template <typename UserClass>
    void bindAction( const std::string& actionName, Input::EventType type, UserClass* instance, void ( UserClass::*handler )() )
    {
        ActionBinding<UserClass>* actionBinding = new ActionBinding<UserClass>( actionName, type, instance, handler );
        m_actionBindings.push_back( actionBinding );
    }
    
    template <typename UserClass>
    void bindAxis( const std::string& axisName, UserClass* instance, void ( UserClass::*handler )( float ) )
    {
        AxisBinding<UserClass>* axisBinding = new AxisBinding<UserClass>( axisName, instance, handler );
        m_axisBindings.push_back( axisBinding );
    }

private:

	std::vector<InputBinding*> m_actionBindings;
	std::vector<InputBinding*> m_axisBindings;
};

