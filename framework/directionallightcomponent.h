// Oleg Kotov

#pragma once

#include "lightsourcecomponent.h"

class ENGINE_API DirectionalLightComponent : public LightSourceComponent
{
public:
    
	DirectionalLightComponent()
	{
		m_lightType = LightType::Directional;
	}
};

