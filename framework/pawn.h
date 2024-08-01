// Oleg Kotov

#pragma once

#include "entity.h"
#include "event.h"

class Controller;

class ENGINE_API Pawn : public Entity
{
	using Super = Entity;

	using PossessedEvent = Event<Controller*>; // newController
	using UnpossessedEvent = Event<Controller*>; // oldController

public:

	Pawn();

	void beginPlay() override;
	void tick( float deltaTime ) override;

	void possess( Controller* newController )
	{
		assert( newController );
		if ( m_controller == newController ) return;

		m_controller = newController;
		possessedEvent( newController );
	}

	void unpossess()
	{
		if ( m_controller )
		{
			unpossessedEvent( m_controller );
			m_controller = nullptr;
		}
	}

private:

	Controller* m_controller = nullptr;

public:

	PossessedEvent possessedEvent;
	UnpossessedEvent unpossessedEvent;
};

