// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "keys.h"
#include "event.h"

#include <vector>
#include <string>

class InputComponent;

struct ENGINE_API ActionKeyMapping
{
	ActionKeyMapping( const std::string& actionName, Input::Key key )
		: m_actionName( actionName )
		, m_key( key )
	{}

	bool operator==( const ActionKeyMapping& other ) const = default;

	std::string m_actionName; // Bind "Jump" action
	Input::Key m_key;         // to "Space" key.
};

struct ENGINE_API AxisKeyMapping
{
	AxisKeyMapping( const std::string& axisName, Input::Key key, float scale )
		: m_axisName( axisName )
		, m_key( key )
		, m_scale( scale )
	{}

	bool operator==( const AxisKeyMapping& other ) const = default;

	std::string m_axisName; // Bind "MoveForward" axis
	Input::Key m_key;       // to "W" and "S" key
    float m_scale;          // with +1 and -1 scale.
};

class ENGINE_API InputSystem
{        
	using KeyPressedEvent = Event<Input::Key>;
	using KeyReleasedEvent = Event<Input::Key>;

public:

	InputSystem();
	~InputSystem();
    
	void addInputComponent( InputComponent* m_component );
	void removeInputComponent( InputComponent* m_component );

    void addActionMapping( const std::string& m_name, Input::Key key );
    void addAxisMapping( const std::string& m_name, Input::Key key, float value );
    
	void getMouseDelta( int& deltaX, int& deltaY ) const;

    void onKeyPressed( Input::Key key );
    void onKeyReleased( Input::Key key );

    void updateKeyAxis();

	bool isKeyPressed( Input::Key key ) const;

private:

	std::vector<ActionKeyMapping> m_actionMappings;
	std::vector<AxisKeyMapping>   m_axisMappings;

	std::vector<InputComponent*> m_inputComponents;

public:

	KeyPressedEvent keyPressedEvent;
	KeyReleasedEvent keyReleasedEvent;
};

// extern ENGINE_API class InputSystem InputSystem;

