// Oleg Kotov

#pragma once

#include "primitivecomponent.h"

class btCollisionShape;

class ENGINE_API ShapeComponent : public PrimitiveComponent
{
	friend class PrimitiveComponent;

public:

	virtual ~ShapeComponent() {}
};

