// Oleg Kotov

#pragma once

#include "shapecomponent.h"

class ENGINE_API BoxComponent : public ShapeComponent
{
public:

	BoxComponent();
	~BoxComponent();

private:

	void createCollisionShape() override;

public:

    void setBoxExtent( const Vector& boxExtent );

private:

	Vector m_boxExtent = Vector( 0.5f, 0.5f, 0.5f );
};

