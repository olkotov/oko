// Oleg Kotov

#pragma once

#include "okobase.h"

class Entity;

/**
* A base class of the entity components.
***********************************************************************************/

class ENGINE_API EntityComponent
{    
public:
    
	// Virtual destructor of the entity component interface.
    virtual ~EntityComponent() {}
    
	// Set the name to an entity component.
    void setName( const std::string& name )
	{
		m_name = name;
	}

	// Get the name of an entity component.
    const std::string& getName() const
	{
		return m_name;
	}
    
	void setEnabled( bool enabled )
	{
		m_enabled = enabled;
	}

	bool isEnabled() const
	{
		return m_enabled;
	}

	// Set the owner to an entity component.
    void setOwner( Entity* owner )
	{
		m_owner = owner;
	}

	// Get the owner of an entity component.
    Entity* getOwner() const
	{
		return m_owner;
	}
    
	// Do some initialization when game is ready to play.
    virtual void beginPlay() {}

	// Do the regular update tick.
    virtual void tick( float deltaTime ) {}

private:

	// The name of the entity component.
	std::string m_name;

	bool m_enabled = true;

	// Owner of the entity component.
	Entity* m_owner = nullptr;
};

