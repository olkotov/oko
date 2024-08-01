// Oleg Kotov

#pragma once

#include "controller.h"

class PlayerController : public Controller
{
	using Super = Entity;

public:

	bool isPlayerController() override
	{
		return true;
	}
};

