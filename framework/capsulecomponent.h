// Oleg Kotov

#pragma once

#include "shapecomponent.h"

class ENGINE_API CapsuleComponent : public ShapeComponent
{
public:

	CapsuleComponent();
    ~CapsuleComponent();
 
private:

	void createCollisionShape() override;

public:

	void setCapsuleSize( float radius, float height );
	void setCapsuleRadius( float radius );
	void setCapsuleHeight( float height );

private:

	float m_radius = 0.5f;
	float m_height = 1.0f;
};

