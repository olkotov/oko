// Oleg Kotov

#pragma once

#include "lightsourcecomponent.h"

class PointLightComponent : public LightSourceComponent
{    
public:
    
	PointLightComponent()
	{
		m_lightType = LightType::Point;
	}

public:

	float m_range = 10.0f;
};

