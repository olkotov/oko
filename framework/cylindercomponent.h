// Oleg Kotov

#pragma once

#include "shapecomponent.h"

class ENGINE_API CylinderComponent : public ShapeComponent
{
public:

	CylinderComponent();
    ~CylinderComponent();
 
private:

	void createCollisionShape() override;

public:

	void setCylinderSize( float radius, float height );
	void setCylinderRadius( float radius );
	void setCylinderHeight( float height );

private:

	float m_radius = 0.5f;
	float m_height = 1.0f;
};

