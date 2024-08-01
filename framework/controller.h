// Oleg Kotov

#pragma once

#include "entity.h"
#include "pawn.h"

class ENGINE_API Controller : public Entity
{
	using Super = Entity;

public:

	virtual void possess( class Pawn* pawn )
	{
		assert( pawn );

		if ( m_controlledPawn )
		{
			m_controlledPawn->unpossess();
		}

		m_controlledPawn = pawn;
		pawn->possess( this );
	}

	virtual void unpossess()
	{
		m_controlledPawn->unpossess();
		m_controlledPawn = nullptr;
	}

	virtual bool isPlayerController() = 0;

private:

	class Pawn* m_controlledPawn = nullptr;
};

