// Oleg Kotov

#pragma once

#include "scenecomponent.h"

class ENGINE_API AudioListener : public SceneComponent
{
public:

	void tick( float deltaTime ) override;
};

