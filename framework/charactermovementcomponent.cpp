// Oleg Kotov

#include "charactermovementcomponent.h"
#include "primitivecomponent.h"
#include "entity.h"
#include "log.h"

void CharacterMovementComponent::beginPlay()
{
	PrimitiveComponent* newUpdatedComponent = dynamic_cast<PrimitiveComponent*>( getOwner()->getRootComponent() );

	if ( !newUpdatedComponent )
	{
		Warning( "Movement component must update a PrimitiveComponent, but owning entity does not have a root PrimitiveComponent. Auto registration failed." );
	}

	setUpdatedComponent( newUpdatedComponent );
}

void CharacterMovementComponent::setUpdatedComponent( PrimitiveComponent* newUpdatedComponent )
{
	if ( !newUpdatedComponent ) return;
	m_updatedComponent = newUpdatedComponent;
}

bool CharacterMovementComponent::isExceedingMaxSpeed( float maxSpeed ) const
{
	maxSpeed = max( 0.0f, maxSpeed );
	const float maxSpeedSq = maxSpeed * maxSpeed;

	// temp
	Vector velocity;

	const float overVelocityPercent = 1.01f;
	return ( velocity.lengthSquared() > maxSpeedSq * overVelocityPercent );
}

void CharacterMovementComponent::setPlaneConstraintEnabled( bool enabled )
{
	m_constrainToPlane = enabled;
}

void CharacterMovementComponent::setPlaneConstraintNormal( Vector planeNormal )
{
	m_planeConstraintNormal = planeNormal.normalized();
}

