// Oleg Kotov

#pragma once

#include "entitycomponent.h"

enum class MovementType : uint8_t
{
	Walk,
	Fall
};

class ENGINE_API CharacterMovementComponent : public EntityComponent
{
public:

	void beginPlay() override;

	// MovementComponent

	void setUpdatedComponent( class PrimitiveComponent* newUpdatedComponent );

	bool isExceedingMaxSpeed( float maxSpeed ) const;

	void setPlaneConstraintEnabled( bool enabled );
	void setPlaneConstraintNormal( Vector planeNormal );

	// NavMovementComponent
	// bool IsFalling();
	// bool IsMovingOnGround();
	
	// virtual bool CheckFall(c
	// VisualizeMovement()

	// virtual bool IsWalkable(const FHitResult& Hit) const;
	bool isWalkable() const;

	float getWalkableFloorAngle() const;
	void setWalkableFloorAngle( float angle );

	float getWalkableFloorZ() const;
	void setWalkableFloorZ( float walkableFloorZ );

private:

	class PrimitiveComponent* m_updatedComponent = nullptr;

	bool m_constrainToPlane = false;
	Vector m_planeConstraintNormal = Vector::zero;

	float m_walkableFloorAngle;
	float m_walkableFloorZ;
};

