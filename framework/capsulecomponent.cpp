// Oleg Kotov

#include "capsulecomponent.h"

#include <bullet/btBulletDynamicsCommon.h>

CapsuleComponent::CapsuleComponent()
{	
	createCollisionShape();
	createBody();
}

CapsuleComponent::~CapsuleComponent()
{
	SafeDelete( m_collisionShape );
}

void CapsuleComponent::createCollisionShape()
{
	if ( m_collisionShape != nullptr )
	{
		SafeDelete( m_collisionShape );
	}

	// height = totalHeight - radius * 2
	m_collisionShape = new btCapsuleShapeZ( m_radius, m_height - m_radius * 2.0f );
	m_collisionShape->setUserPointer( this );
}

void CapsuleComponent::setCapsuleSize( float radius, float height )
{
	m_radius = radius;
	m_height = height;

	createCollisionShape();
	createBody();
}

void CapsuleComponent::setCapsuleRadius( float radius )
{
	m_radius = radius;

	createCollisionShape();
	createBody();
}

void CapsuleComponent::setCapsuleHeight( float height )
{
	m_height = height;

	createCollisionShape();
	createBody();
}

