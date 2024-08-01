// Oleg Kotov

#include "entity.h"
#include "entitycomponent.h"
#include "scenecomponent.h"
#include "primitivecomponent.h"
#include "world.h"
#include "log.h"

// std::string Entity::m_classname = "";

Entity::~Entity()
{
    // remove from world
    // remove components

    for ( auto& component : m_components )
    {
        // component->endPlay(); ???
        SafeDelete( component );
    }
}

void Entity::setName( const std::string& name )
{
    m_name = name;
}

const std::string& Entity::getName() const
{
    return ( !m_name.empty() ) ? m_name : m_classname;
}

const std::string& Entity::getClassname() const
{
    return m_classname;
}

void Entity::addComponent( EntityComponent* component )
{
    if ( component == nullptr ) return;

    m_components.push_back( component );
    component->setOwner( this );
    component->beginPlay();
}

const std::vector<EntityComponent*>& Entity::getComponents() const
{
    return m_components;
}

void Entity::beginPlay()
{
    for ( const auto& component : m_components )
    {
        component->beginPlay();
    }
}

void Entity::tick( float deltaTime )
{
    if ( m_pendingKill ) return;

    for ( const auto& component : m_components )
    {
        if ( !component->isEnabled() ) continue;
        component->tick( deltaTime );
    }
}

void Entity::endPlay()
{
    for ( const auto& component : m_components )
    {
        // component->endPlay();
    }
}

void Entity::destroy()
{
    // TODO:
    // destroy entity

    // m_world->removeEntity( this );
    m_pendingKill = true;
    // delete this;
}

const Matrix& Entity::getWorldTransform()
{
    if ( !m_rootComponent ) return Matrix::Identity;
    return m_rootComponent->getWorldTransform();
}

void Entity::setWorldPosition( const Vector& location )
{
    if ( !m_rootComponent ) return;
    
    m_rootComponent->setWorldPosition( location );

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( physics )
    {
        physics->syncComponentToRigidBody();
    }
}

void Entity::setWorldEulerAngles( const Vector& angles )
{
    if ( !m_rootComponent ) return;
    m_rootComponent->setWorldRotation( Quaternion::makeFromEuler( angles ) );
}

void Entity::setWorldRotation( const Quaternion& rotation )
{
    if ( !m_rootComponent ) return;
    m_rootComponent->setWorldRotation( rotation );
}

void Entity::translate( const Vector& translation )
{
    if ( !m_rootComponent ) return;
    m_rootComponent->translate( translation );
}

void Entity::rotate( const Vector& rotation )
{
    if ( !m_rootComponent ) return;
    m_rootComponent->rotate( rotation );
}

void Entity::rotate( const Quaternion& rotation )
{
    if ( !m_rootComponent ) return;
    m_rootComponent->rotate( rotation );
}

Vector Entity::getWorldPosition() const
{
	if ( !m_rootComponent ) return Vector::zero;
	return m_rootComponent->getWorldPosition();
}

Vector Entity::getWorldEulerAngles() const
{
	if ( !m_rootComponent ) return Vector::zero;
	return m_rootComponent->getWorldEulerAngles();
}

Quaternion Entity::getWorldRotation() const
{
	if ( !m_rootComponent ) return Quaternion::Identity;
    return m_rootComponent->getWorldRotation();
}

void Entity::setRelativePosition( const Vector& position ) const
{
    if ( !m_rootComponent ) return;
    m_rootComponent->setRelativePosition( position );
}

void Entity::setRelativeEulerAngles( const Vector& angles ) const
{
    if ( !m_rootComponent ) return;
    m_rootComponent->setRelativeRotation( Quaternion::makeFromEuler( angles ) );
}

void Entity::setRelativeScale( const Vector& scale ) const
{
    if ( !m_rootComponent ) return;
    m_rootComponent->setRelativeScale( scale );
}

Vector Entity::getRelativeEulerAngles() const
{
    if ( !m_rootComponent ) return Vector::zero;
	return m_rootComponent->getRelativeEulerAngles();
}

void Entity::translateLocal( const Vector& translation ) const
{
    if ( !m_rootComponent ) return;

    m_rootComponent->translateLocal( translation );

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( physics )
    {
        physics->syncComponentToRigidBody();
    }
}

void Entity::rotateLocal( const Vector& rotation )
{
    if ( !m_rootComponent ) return;

    m_rootComponent->rotateLocal( rotation );

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( physics )
    {
        physics->syncComponentToRigidBody();
    }
}

void Entity::rotateLocal( const Quaternion& rotation )
{
    if ( !m_rootComponent ) return;

    m_rootComponent->rotateLocal( rotation );

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( physics )
    {
        physics->syncComponentToRigidBody();
    }
}

Vector Entity::getUpDirection()
{
	if ( !m_rootComponent ) return Vector::zero;
	return m_rootComponent->getUpDirection();
}

Vector Entity::getRightDirection()
{
	if ( !m_rootComponent ) return Vector::zero;
	return m_rootComponent->getRightDirection();
}

Vector Entity::getForwardDirection()
{
	if ( !m_rootComponent ) return Vector::zero;
	return m_rootComponent->getForwardDirection();
}

void Entity::addForce( const Vector& force ) const
{
    if ( !m_rootComponent ) return;

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( physics )
    {
        physics->addForce( force );
    }
}

void Entity::addForceAtLocation( const Vector& force, const Vector& location ) const
{
    if ( !m_rootComponent ) return;

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( physics )
    {
        physics->addForceAtLocation( force, location );
    }
}

void Entity::applyImpulse( const Vector& impulse ) const
{
    if ( !m_rootComponent ) return;

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( physics )
    {
        physics->addImpulse( impulse );
    }
}

void Entity::addImpulseAtLocation( const Vector& impulse, const Vector& location ) const
{
    if ( !m_rootComponent ) return;

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( physics )
    {
        physics->addImpulseAtLocation( impulse, location );
    }
}

void Entity::setLinearVelocity( const Vector& velocity ) const
{
    if ( !m_rootComponent ) return;

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( physics )
    {
        physics->setLinearVelocity( velocity );
    }
}

Vector Entity::getLinearVelocity() const
{
    if ( !m_rootComponent ) return Vector::zero;

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( !physics ) return Vector::zero;

    return physics->getLinearVelocity();
}

void Entity::setAngularVelocity(const Vector& velocity) const
{
    if ( !m_rootComponent ) return;

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( physics != nullptr )
    {
        physics->setAngularVelocity( velocity );
    }
}

Vector Entity::getAngularVelocity() const
{
    if ( !m_rootComponent ) return Vector::zero;

    PrimitiveComponent* physics = dynamic_cast<PrimitiveComponent*>( m_rootComponent );

    if ( !physics ) return Vector::zero;

    return physics->getAngularVelocity();
}

