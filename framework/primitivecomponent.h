// Oleg Kotov

#pragma once

#include "scenecomponent.h"

class btRigidBody;
class btCollisionShape;
class btTransform;

enum class BodyType // PhysicsMotionType
{
	Kinematic, // Keyframed
	Dynamic
	// Static
};

class ENGINE_API PrimitiveComponent : public SceneComponent
{    
public:
    
    PrimitiveComponent();
    ~PrimitiveComponent();

	void printBodyType() const;

	// Enables or disables physics simulation for the physical object.
	void setSimulatePhysics( bool simulate );

	bool getSimulationPhysics() const;

	void setCollisionEnabled( bool enabled );
	bool getCollisionEnabled() const;

	void setGravityScale( float scale );
	float getGravityScale() const;

	void setEnableGravity( bool gravityEnabled );
	bool isGravityEnabled() const;

	// Sets the mass of a physical object in kilograms.
    void setMass( float mass );

	// Retrieves the mass of the physical object in kilograms.
    float getMass() const;

	void setCenterOfMass( const Vector& centerOfMass );
	Vector getCenterOfMass() const;

	void setLinearVelocity( const Vector& velocity );
	void setAngularVelocity( const Vector& angularVelocity );

	void addImpulse( const Vector& impulse ) const;
	void addImpulseAtLocation( const Vector& impulse, const Vector& location ) const;
	void addForce( const Vector& force );

	// add force at relative location
	void addForceAtLocation( const Vector& force, const Vector& location );

	void setFriction( float friction );
	float getFriction() const;

	void setRestitution( float restitution );
	float getRestitution() const;

	void setLinearDamping( float linearDamping );
	float getLinearDamping() const;

	void setAngularDamping( float angularDamping );
	float getAngularDamping() const;

	void setLinearFactor( const Vector& linearFactor );
	const Vector& getLinearFactor() const;

	void setAngularFactor( const Vector& angularFactor );
	const Vector& getAngularFactor() const;

	Vector getLinearVelocity() const;
	Vector getLinearVelocityAtPoint( const Vector& point ) const;

	Vector getAngularVelocity() const;

	void disableSimulation();
	void enableSimulation();

//    // Unused
//    // Vector GetPhysicsAngularVelocityInDegrees();

	void syncComponentToRigidBody();

protected:

	void createBody();
	void createRigidBody( float mass, btCollisionShape* shape, btTransform transform );

private:

	BodyType m_bodyType = BodyType::Kinematic;
	bool m_isTrigger = false;

	Vector m_gravity = Vector( 0.0f, 0.0f, -9.81f );
	float m_gravityScale = 1.0f;

	float m_mass = 1.0f;

	float  m_friction = 0.5f;
	float  m_restitution = 0.5f;
	float  m_linearDamping = 0.0f;
	float  m_angularDamping = 0.0f;
	Vector m_linearFactor = Vector::one;
	Vector m_angularFactor = Vector::one;

	btTransform getComponentTransform();

protected:

	btRigidBody* m_rigidBody = nullptr;
	btCollisionShape* m_collisionShape = nullptr;

	virtual void createCollisionShape() = 0;
};

