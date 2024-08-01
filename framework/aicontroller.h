// Oleg Kotov

#pragma once

#include "controller.h"

class AIController : public Controller
{
	using Super = Entity;

public:

	bool isPlayerController() override
	{
		return false;
	}
};

