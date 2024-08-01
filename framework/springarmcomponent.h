// Oleg Kotov

#pragma once

#include "scenecomponent.h"

// attachment slot

class SpringArmComponent : public SceneComponent
{    
public:

    void tick( float deltaTime ) override;
    
public:

	float m_targetArmLength = 3.0f;
};

