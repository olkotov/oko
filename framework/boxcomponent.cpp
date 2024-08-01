// Oleg Kotov

#include "boxcomponent.h"

#include <bullet/btBulletDynamicsCommon.h>

BoxComponent::BoxComponent()
{
    createCollisionShape();
    createBody();
}

BoxComponent::~BoxComponent()
{
    SafeDelete( m_collisionShape );
}

void BoxComponent::createCollisionShape()
{
    if ( m_collisionShape )
    {
        SafeDelete( m_collisionShape );
    }

    m_collisionShape = new btBoxShape( btVector3( m_boxExtent.x, m_boxExtent.y, m_boxExtent.z ) );
    m_collisionShape->setUserPointer( this );
}

void BoxComponent::setBoxExtent( const Vector& boxExtent )
{
    m_boxExtent = boxExtent;

    createCollisionShape();
    createBody();
}

