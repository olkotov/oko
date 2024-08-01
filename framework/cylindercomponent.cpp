// Oleg Kotov

#include "cylindercomponent.h"
#include <bullet/btBulletDynamicsCommon.h>

CylinderComponent::CylinderComponent()
{	
	createCollisionShape();
	createBody();
}

CylinderComponent::~CylinderComponent()
{
	SafeDelete( m_collisionShape );
}

void CylinderComponent::createCollisionShape()
{
	if ( m_collisionShape )
	{
		SafeDelete( m_collisionShape );
	}

	btVector3 halfExtents( m_radius, m_radius, m_height * 0.5f );
	m_collisionShape = new btCylinderShapeZ( halfExtents );
	m_collisionShape->setUserPointer( this );
}

void CylinderComponent::setCylinderSize( float radius, float height )
{
	m_radius = radius;
	m_height = height;

	createCollisionShape();
	createBody();
}

void CylinderComponent::setCylinderRadius( float radius )
{
	m_radius = radius;

	createCollisionShape();
	createBody();
}

void CylinderComponent::setCylinderHeight( float height )
{
	m_height = height;

	createCollisionShape();
	createBody();
}

