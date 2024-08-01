// Oleg Kotov

#pragma once

#include "pointlightcomponent.h"

class ENGINE_API SpotLightComponent : public PointLightComponent
{    
public:
    
	SpotLightComponent()
	{
		m_lightType = LightType::Spot;
	}

public:

	float m_innerConeAngle = 40.0f;
	float m_outerConeAngle = 45.0f;
};

