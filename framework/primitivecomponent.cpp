// Oleg Kotov

#include "primitivecomponent.h"
#include "shapecomponent.h"
#include "physicsengine.h"
#include "okoengine.h"
#include "log.h"

#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>

PrimitiveComponent::PrimitiveComponent()
{

}

PrimitiveComponent::~PrimitiveComponent()
{
	auto physicsEngine = OkoEngine::instance()->physics();
	btDynamicsWorld* physicsWorld = physicsEngine->getBulletPhysicsWorld();

    // physicsWorld->removeRigidBody( m_rigidBody );
    
    disableSimulation();

    SafeDelete( m_rigidBody );

    physicsEngine->removeComponentFromSystem( this );
}

void PrimitiveComponent::printBodyType() const
{
    if ( m_rigidBody->isStaticOrKinematicObject() )
	{
		Msg( "static or kinematic" );

		if ( m_rigidBody->isStaticObject() )
		{
			Msg( "static" );
		}
		else
		{
			Msg( "kinematic" );
		}
	}
	else
	{
		Msg( "dynamic" );
	}
}

// TODO: update mass?
// check playcanvas as example
void PrimitiveComponent::setSimulatePhysics( bool simulate )
{
    if ( !m_rigidBody ) return;
    
    disableSimulation();

    if ( simulate )
    {
        m_bodyType = BodyType::Dynamic;

        m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT );
        m_rigidBody->forceActivationState( ACTIVE_TAG ); // ISLAND_SLEEPING
    }
    else
    {
        m_bodyType = BodyType::Kinematic;
        
        m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
        m_rigidBody->forceActivationState( DISABLE_DEACTIVATION );
    }

    enableSimulation();
}

bool PrimitiveComponent::getSimulationPhysics() const
{
    return m_bodyType == BodyType::Dynamic;
}

void PrimitiveComponent::setCollisionEnabled( bool enabled )
{
    if ( !m_rigidBody ) return;
    
    disableSimulation();

    if ( enabled )
    {
        m_isTrigger = false;
        m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE );
    }
    else
    {
        m_isTrigger = true;
        m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );
    }

    enableSimulation();
}

bool PrimitiveComponent::getCollisionEnabled() const
{
    return !m_isTrigger;
}

void PrimitiveComponent::setGravityScale( float scale )
{
    m_gravityScale = scale;

    auto physicsEngine = OkoEngine::instance()->physics();
    btDynamicsWorld* physicsWorld = physicsEngine->getBulletPhysicsWorld();

    btVector3 worldGravity = physicsWorld->getGravity();

    m_rigidBody->setGravity( worldGravity * m_gravityScale );
}

float PrimitiveComponent::getGravityScale() const
{
    return m_gravityScale;
}

void PrimitiveComponent::setEnableGravity( bool gravityEnabled )
{
    m_rigidBody->setGravity( ( gravityEnabled == true ) ? btVector3( m_gravity.x, m_gravity.y, m_gravity.z ) : btVector3( 0.0f, 0.0f, 0.0f ) );
}

bool PrimitiveComponent::isGravityEnabled() const
{
    return !m_rigidBody->getGravity().isZero();
}

void PrimitiveComponent::setMass( float mass )
{
    float bodyMass = m_rigidBody->getMass();

    if ( bodyMass == mass ) return;
    if ( m_rigidBody->isStaticOrKinematicObject() ) return;
    if ( mass < 0.0f ) mass = 0.0f;

    disableSimulation();

    btVector3 localInertia;

    btCollisionShape* collisionShape = m_rigidBody->getCollisionShape();

    if ( mass > 0.0f )
    {
        collisionShape->calculateLocalInertia( mass, localInertia );
    }

    m_rigidBody->setMassProps( mass, localInertia );
    m_rigidBody->updateInertiaTensor();
    
    enableSimulation();
}

float PrimitiveComponent::getMass() const
{
    return m_rigidBody->getMass();
}

void PrimitiveComponent::setCenterOfMass( const Vector& centerOfMass )
{
    if ( !m_rigidBody ) return;
    
    btTransform transform;
    transform.setOrigin( btVector3( centerOfMass.x, centerOfMass.y, centerOfMass.z ) );

    m_rigidBody->setCenterOfMassTransform( transform );
}

Vector PrimitiveComponent::getCenterOfMass() const
{
    if ( !m_rigidBody ) return Vector::zero;
    const btVector3& com = m_rigidBody->getCenterOfMassPosition();
    return Vector( com.x(), com.y(), com.z() );
}

void PrimitiveComponent::setLinearVelocity( const Vector& velocity )
{
    if ( !m_rigidBody ) return;
    m_rigidBody->activate( true );
    m_rigidBody->setLinearVelocity( btVector3( velocity.x, velocity.y, velocity.z ) );
}

void PrimitiveComponent::setAngularVelocity( const Vector& velocity )
{
    if ( !m_rigidBody ) return;
    m_rigidBody->setAngularVelocity( btVector3( velocity.x, velocity.y, velocity.z ) );
}

void PrimitiveComponent::addImpulse( const Vector& impulse ) const
{
    if ( !m_rigidBody ) return;
    m_rigidBody->applyCentralImpulse( btVector3( impulse.x, impulse.y, impulse.z ) );
}

void PrimitiveComponent::addImpulseAtLocation( const Vector& impulse, const Vector& location ) const
{
    if ( !m_rigidBody ) return;
    m_rigidBody->applyImpulse( btVector3( impulse.x, impulse.y, impulse.z ), btVector3( location.x, location.y, location.z ) );
}

void PrimitiveComponent::addForce( const Vector& force )
{
    if ( !m_rigidBody ) return;
    m_rigidBody->activate();
    m_rigidBody->applyCentralForce( btVector3( force.x, force.y, force.z ) );
}

void PrimitiveComponent::addForceAtLocation( const Vector& force, const Vector& location )
{
    if ( !m_rigidBody ) return;
    m_rigidBody->applyForce( btVector3( force.x, force.y, force.z ), btVector3( location.x, location.y, location.z ) );
}

void PrimitiveComponent::setFriction( float friction )
{
    m_friction = friction;
    if ( !m_rigidBody ) return;
    m_rigidBody->setFriction( friction );
}

float PrimitiveComponent::getFriction() const
{
    return m_friction;
}

void PrimitiveComponent::setRestitution( float restitution )
{
    m_restitution = restitution;
    if ( !m_rigidBody ) return;
    m_rigidBody->setRestitution( restitution );
}

float PrimitiveComponent::getRestitution() const
{
    return m_restitution;
}

void PrimitiveComponent::setLinearDamping( float linearDamping )
{
    m_linearDamping = linearDamping;
    if ( !m_rigidBody ) return;
    m_rigidBody->setDamping( linearDamping, m_rigidBody->getAngularDamping() );
}

float PrimitiveComponent::getLinearDamping() const
{
    return m_linearDamping;
}

void PrimitiveComponent::setAngularDamping( float angularDamping )
{
    m_angularDamping = angularDamping;
    if ( !m_rigidBody ) return;
    m_rigidBody->setDamping( m_linearDamping, angularDamping );
}

float PrimitiveComponent::getAngularDamping() const
{
    return m_angularDamping;
}

void PrimitiveComponent::setLinearFactor( const Vector& linearFactor )
{
    m_linearFactor = linearFactor;
    if ( !m_rigidBody || m_bodyType != BodyType::Dynamic ) return;
    m_rigidBody->setLinearFactor( btVector3( linearFactor.x, linearFactor.y, linearFactor.z ) );
}

const Vector& PrimitiveComponent::getLinearFactor() const
{
    return m_linearFactor;
}

void PrimitiveComponent::setAngularFactor( const Vector& angularFactor )
{
    m_angularFactor = angularFactor;
    if ( !m_rigidBody || m_bodyType != BodyType::Dynamic ) return;
    m_rigidBody->setAngularFactor( btVector3( angularFactor.x, angularFactor.y, angularFactor.z ) );
}

const Vector& PrimitiveComponent::getAngularFactor() const
{
    return m_angularFactor;
}

Vector PrimitiveComponent::getLinearVelocity() const
{
    if ( !m_rigidBody ) return Vector::zero;
    
    const btVector3& velocity = m_rigidBody->getLinearVelocity();
    return Vector( velocity.x(), velocity.y(), velocity.z() );
}

// https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=11924
Vector PrimitiveComponent::getLinearVelocityAtPoint( const Vector& point ) const
{
    if ( !m_rigidBody ) return Vector::zero;
    
    // m_rigidBody->getVelocityInLocalPoint();

    btVector3 lever = btVector3( point.x, point.y, point.z ) - m_rigidBody->getCenterOfMassPosition();
    btVector3 velocity = m_rigidBody->getLinearVelocity() + btCross( m_rigidBody->getAngularVelocity(), lever );

    return Vector( velocity.x(), velocity.y(), velocity.z() );
}


Vector PrimitiveComponent::getAngularVelocity() const
{
    if ( !m_rigidBody ) return Vector::zero;

    const btVector3& velocity = m_rigidBody->getAngularVelocity();
    return Vector( velocity.x(), velocity.y(), velocity.z() );
}

btTransform PrimitiveComponent::getComponentTransform()
{
    btTransform worldTransform;

    Vector p = getWorldPosition();
    Quaternion q = getWorldRotation();

    worldTransform.setOrigin( btVector3( p.x, p.y, p.z ) );
    worldTransform.setRotation( btQuaternion( q.x, q.y, q.z, q.w ) );

    return worldTransform;
}

void PrimitiveComponent::syncComponentToRigidBody()
{
    if ( !m_rigidBody ) return;

    btTransform worldTransform = getComponentTransform();
    m_rigidBody->setWorldTransform( worldTransform );

    if ( m_rigidBody->isKinematicObject() )
    {
        btMotionState* motionState = m_rigidBody->getMotionState();

        if ( motionState )
        {
            motionState->setWorldTransform( worldTransform );
        }
    }

    m_rigidBody->activate();
}

// remove from physics world and set activation state

void PrimitiveComponent::disableSimulation()
{
    if ( !m_rigidBody ) return;

    btDynamicsWorld* physicsWorld = engine()->physics()->getBulletPhysicsWorld();

    bool isInWorld = m_rigidBody->isInWorld();

    if ( isInWorld )
    {
        physicsWorld->removeRigidBody( m_rigidBody );
        m_rigidBody->forceActivationState( DISABLE_SIMULATION );
    }
}

// add to physics world, set activation state and activate

void PrimitiveComponent::enableSimulation()
{
    assert( m_rigidBody );

    btDynamicsWorld* physicsWorld = engine()->physics()->getBulletPhysicsWorld();
    physicsWorld->addRigidBody( m_rigidBody );

    if ( m_bodyType == BodyType::Dynamic )
    {
        m_rigidBody->forceActivationState( ACTIVE_TAG );
        syncComponentToRigidBody();
    }
    else if ( m_bodyType == BodyType::Kinematic )
    {
        m_rigidBody->forceActivationState( DISABLE_DEACTIVATION );
    }

    m_rigidBody->activate();
}

// call create rigid body and init from component data
// if rigid body exist, destroy it and recreate

void PrimitiveComponent::createBody()
{
    if ( m_rigidBody )
    {
        btDynamicsWorld* physicsWorld = engine()->physics()->getBulletPhysicsWorld();
        physicsWorld->removeRigidBody( m_rigidBody );
        SafeDelete( m_rigidBody );
    }

    float mass = ( m_bodyType == BodyType::Dynamic ) ? m_mass : 0.0f;

    if ( !m_collisionShape ) return;

    btTransform worldTransform = getComponentTransform();

    createRigidBody( mass, m_collisionShape, worldTransform );

    m_rigidBody->setFriction( m_friction );
    m_rigidBody->setRestitution( m_restitution );
    // m_rigidBody->setRollingFriction();
    m_rigidBody->setDamping( m_linearDamping, m_angularDamping );

    if ( m_bodyType == BodyType::Dynamic )
    {
        m_rigidBody->setLinearFactor( btVector3( m_linearFactor.x, m_linearFactor.y, m_linearFactor.z ) );
        m_rigidBody->setAngularFactor( btVector3( m_angularFactor.x, m_angularFactor.y, m_angularFactor.z ) );
    }
    else if ( m_bodyType == BodyType::Kinematic )
    {
        m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
        m_rigidBody->forceActivationState( DISABLE_DEACTIVATION );
    }

    enableSimulation();
}

// create rigid body, not init

void PrimitiveComponent::createRigidBody( float mass, btCollisionShape* shape, btTransform transform )
{
	btDefaultMotionState* motionState = new btDefaultMotionState( transform );
    
    btVector3 localInertia( 0.0f, 0.0f, 0.0f );
    
    if ( mass != 0.0f )
    {
        shape->calculateLocalInertia( mass, localInertia );
    }
    
    btRigidBody::btRigidBodyConstructionInfo bodyInfo( mass, motionState, shape, localInertia );

    m_rigidBody = new btRigidBody( bodyInfo );
    m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT );
    m_rigidBody->setUserPointer( this );

    if ( mass != 0.0f )
    {
        m_rigidBody->setMassProps( mass, localInertia );
        m_rigidBody->updateInertiaTensor();
    }
}

