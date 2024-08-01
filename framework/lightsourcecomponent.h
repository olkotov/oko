// Oleg Kotov

#pragma once

#include "scenecomponent.h"

/**
* Enumeration for the type of the light source.
***********************************************************************************/

enum class LightType : uint8
{
	Directional,
	Point,
	Spot,
	Undefined
};

/**
* Represents a light source component.
***********************************************************************************/

class ENGINE_API LightSourceComponent : public SceneComponent
{
public:

	LightType getLightType() const
	{
		return m_lightType;
	}

public:

    Color m_lightColor = Color::White;
    float m_intensity = 1.0f;

protected:

	LightType m_lightType = LightType::Undefined;
};

