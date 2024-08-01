// Oleg Kotov

#include "spherecomponent.h"

#include <bullet/btBulletDynamicsCommon.h>

SphereComponent::SphereComponent()
{
    createCollisionShape();
    createBody();
}

SphereComponent::~SphereComponent()
{
    delete m_collisionShape;
    m_collisionShape = nullptr;
}

void SphereComponent::createCollisionShape()
{
    if ( m_collisionShape != nullptr )
    {
        delete m_collisionShape;
        m_collisionShape = nullptr;
    }
    
    m_collisionShape = new btSphereShape( m_radius );
    m_collisionShape->setUserPointer( this );
}

void SphereComponent::setSphereRadius( float radius )
{
    m_radius = radius;

    createCollisionShape();
    createBody();
}

