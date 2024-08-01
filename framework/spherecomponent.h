// Oleg Kotov

#pragma once

#include "shapecomponent.h"

class ENGINE_API SphereComponent : public ShapeComponent
{    
public:
    
	SphereComponent();
    ~SphereComponent();
    
private:

    void createCollisionShape() override;

public:

    void setSphereRadius( float radius );

private:

	float m_radius = 0.5f;
};

