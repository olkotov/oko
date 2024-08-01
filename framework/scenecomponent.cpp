// Oleg Kotov

#include "scenecomponent.h"
#include "transformmatrix.h"

#include "primitivecomponent.h"

bool SceneComponent::isChildOf( SceneComponent* possibleParent )
{
	SceneComponent* currentParent = m_parent;
    
	while ( currentParent )
	{
		if ( currentParent == possibleParent ) return true;
        currentParent = currentParent->getParent();
	}

	return false;
}

void SceneComponent::attachTo( SceneComponent* newParent )
{
    // Если новый родительский компонент не существует, выходим из функции.
    if ( !newParent ) return;
    
    // Если новый родительский компонент уже является родительским для текущего компонента, выходим из функции.
    if ( m_parent == newParent ) return;
    
    // Если текущий компонент пытается стать своим собственным родительским компонентом, выходим из функции.
    if ( m_parent == this ) return;

    // Если новый родительский компонент является дочерним для текущего компонента,
    // то установка нового родительского компонента приведет к циклическому связыванию иерархии компонентов.
    // Поэтому выходим из функции.
    if ( newParent->isChildOf( this ) ) return;
    
    // Если текущий компонент уже имеет родительский компонент,
    // то удаляем его из списка дочерних компонентов старого родительского компонента.
    if ( m_parent )
    {
		m_parent->m_children.erase( std::find( m_children.begin(), m_children.end(), this ) );
    }
    
    // Устанавливаем новый родительский компонент как родительский компонент текущего компонента.
    m_parent = newParent;

    // Добавляем текущий компонент в список дочерних компонентов нового родительского компонента.
    m_parent->m_children.push_back( this );

    markLocalTransformDirty(); // temp?
}

SceneComponent* SceneComponent::getParent() const
{
    return m_parent;
}

//void SceneComponent::UpdateComponentToWorld()
//{
//    // if (!componentToWorldUpdated)
//    {
//        Matrix relativeTransform = GetRelativeTransform();
//        
//        if ( _parent == nullptr )
//        {
//            _componentToWorld = relativeTransform;
//        }
//        else
//        {
//            _componentToWorld = _parent->GetComponentTransform() * relativeTransform;
//        }
//
//        _isComponentToWorldUpdated = true;
//    }
//}

void SceneComponent::setWorldTransform( const Matrix& matrix )
{
    m_dirtyWorld = true;
    m_localToWorldMatrix = matrix;
}

const Matrix& SceneComponent::getWorldTransform()
{
    if ( !m_dirtyLocal && !m_dirtyWorld  )
    {
        return m_localToWorldMatrix;
    }
    
    if ( m_parent )
    {
        // trigger for update the parent component transform
        m_parent->getWorldTransform();
    }

    recalculateMatrices();
    
    return m_localToWorldMatrix;
}

const Matrix& SceneComponent::getWorldToLocalMatrix()
{
    getWorldTransform(); // trigger m_localToWorldMatrix update
    return m_worldToLocalMatrix;
}

const Matrix& SceneComponent::getWorldMatrixInverseTranspose()
{
	getWorldTransform(); // trigger m_localToWorldMatrix update
	return m_worldMatrixInverseTranspose;
}

const Matrix& SceneComponent::getLocalMatrix()
{
    if ( m_dirtyLocal == true )
    {
        m_localMatrix = TransformMatrix( m_localPosition, m_localRotation, m_localScale );
        m_dirtyLocal = false;
    }

    return m_localMatrix;
}

void SceneComponent::setWorldPositionAndRotation( const Vector& position, const Quaternion& rotation, bool teleportPhysics /* = true */ )
{
    setWorldRotation( rotation, teleportPhysics );
    setWorldPosition( position, teleportPhysics );
}

void SceneComponent::setWorldPosition( const Vector& position, bool teleportPhysics /* = true */ )
{
    m_localPosition = position;

    if ( m_parent )
    {
        Matrix invParentTransform = m_parent->getWorldTransform().inverse();
        m_localPosition = invParentTransform.transformPoint( position );
    }

    if ( m_dirtyLocal == false )
    {
        markLocalTransformDirty();
    }

    if ( teleportPhysics == true )
    {
        PrimitiveComponent* physicsComponent = dynamic_cast<PrimitiveComponent*>( this );
        
        if ( physicsComponent != nullptr )
        {
            physicsComponent->syncComponentToRigidBody();
        }
    }
}

void SceneComponent::setWorldEulerAngles( const Vector& eulerAngles, bool teleportPhysics /* = true */ )
{
    setWorldRotation( Quaternion::makeFromEuler( eulerAngles ), teleportPhysics );
}

void SceneComponent::setWorldRotation( const Quaternion& rotation, bool teleportPhysics /* = true */ )
{
    if ( !m_parent )
    {
        m_localRotation = rotation;
    }
    else
    {
        m_localRotation = m_parent->getWorldRotation().inverse() * rotation;
    }

    if ( !m_dirtyLocal )
    {
        markLocalTransformDirty();
    }

    if ( teleportPhysics )
    {
        PrimitiveComponent* physicsComponent = dynamic_cast<PrimitiveComponent*>( this );
        
        if ( physicsComponent )
        {
            physicsComponent->syncComponentToRigidBody();
        }
    }
}

void SceneComponent::translate( const Vector& translation, bool teleportPhysics /* = true */ )
{
    setWorldPosition( getWorldPosition() + translation, teleportPhysics );
}

void SceneComponent::rotate( const Vector& rotation, bool teleportPhysics /* = true */ )
{
    Quaternion deltaRotation = Quaternion::makeFromEuler( rotation );
    rotate( deltaRotation, teleportPhysics );
}

void SceneComponent::rotate( const Quaternion& deltaRotation, bool teleportPhysics /* = true */ )
{        
    if ( m_parent == nullptr )
    {
        m_localRotation = deltaRotation * m_localRotation;
    }
    else
    {
        Quaternion invParentRotation = m_parent->getWorldRotation().inverse();
        m_localRotation = invParentRotation * deltaRotation * getWorldRotation();
    }

    if ( m_dirtyLocal == false )
    {
        markLocalTransformDirty();
    }

    if ( teleportPhysics == true )
    {
        PrimitiveComponent* physicsComponent = dynamic_cast<PrimitiveComponent*>( this );
        
        if ( physicsComponent != nullptr )
        {
            physicsComponent->syncComponentToRigidBody();
        }
    }
}

Vector SceneComponent::getWorldPosition()
{
    return getWorldTransform().getTranslation();
}

Vector SceneComponent::getWorldEulerAngles()
{
    return getWorldTransform().getRotation().getEulerAngles();
}

Quaternion SceneComponent::getWorldRotation()
{
    return getWorldTransform().getRotation();
}

void SceneComponent::setRelativePositionAndRotation( const Vector& position, const Quaternion& rotation, bool teleportPhysics /* = true */ )
{
    m_localPosition = position;
    m_localRotation = rotation;

    if ( m_dirtyLocal == false )
    {
        markLocalTransformDirty();
    }

    if ( teleportPhysics == true )
    {
        PrimitiveComponent* physicsComponent = dynamic_cast<PrimitiveComponent*>( this );
        
        if ( physicsComponent != nullptr )
        {
            physicsComponent->syncComponentToRigidBody();
        }
    }
}

void SceneComponent::setRelativePosition( const Vector& position, bool teleportPhysics /* = true */ )
{
    m_localPosition = position;

    if ( !m_dirtyLocal )
    {
        markLocalTransformDirty();
    }

    if ( teleportPhysics )
    {
        PrimitiveComponent* physicsComponent = dynamic_cast<PrimitiveComponent*>( this );
        
        if ( physicsComponent )
        {
            physicsComponent->syncComponentToRigidBody();
        }
    }
}

void SceneComponent::setRelativeEulerAngles( const Vector& eulerAngles, bool teleportPhysics /* = true */ )
{
    setRelativeRotation( Quaternion::makeFromEuler( eulerAngles ), teleportPhysics );
}

void SceneComponent::setRelativeRotation( const Quaternion& rotation, bool teleportPhysics /* = true */ )
{
    m_localRotation = rotation;

    if ( m_dirtyLocal == false )
    {
        markLocalTransformDirty();
    }

    if ( teleportPhysics == true )
    {
        PrimitiveComponent* physicsComponent = dynamic_cast<PrimitiveComponent*>( this );
        
        if ( physicsComponent != nullptr )
        {
            physicsComponent->syncComponentToRigidBody();
        }
    }
}

void SceneComponent::setRelativeScale( const Vector& scale )
{
    m_localScale = scale;

    if ( !m_dirtyLocal )
    {
        markLocalTransformDirty();
    }
}

void SceneComponent::translateLocal( const Vector& translation, bool teleportPhysics /* = true */ )
{
    m_localPosition += m_localRotation.transformVector( translation );

    if ( !m_dirtyLocal )
    {
        markLocalTransformDirty();
    }

    if ( teleportPhysics )
    {
        PrimitiveComponent* physicsComponent = dynamic_cast<PrimitiveComponent*>( this );
        
        if ( physicsComponent )
        {
            physicsComponent->syncComponentToRigidBody();
        }
    }
}

void SceneComponent::rotateLocal( const Vector& rotation, bool teleportPhysics /* = true */ )
{
    rotateLocal( Quaternion::makeFromEuler( rotation ), teleportPhysics );
}

void SceneComponent::rotateLocal( const Quaternion& rotation, bool teleportPhysics /* = true */ )
{
    m_localRotation *= rotation;

    if ( !m_dirtyLocal )
    {
        markLocalTransformDirty();
    }

    if ( teleportPhysics )
    {
        PrimitiveComponent* physicsComponent = dynamic_cast<PrimitiveComponent*>( this );
        
        if ( physicsComponent )
        {
            physicsComponent->syncComponentToRigidBody();
        }
    }
}

Vector SceneComponent::getRelativePosition() const
{
    return m_localPosition;
}

Vector SceneComponent::getRelativeEulerAngles() const
{
    return m_localRotation.getEulerAngles();
}

Quaternion SceneComponent::getRelativeRotation() const
{
    return m_localRotation;
}

Vector SceneComponent::GetRelativeScale() const
{
    return m_localScale;
}

Vector SceneComponent::getForwardDirection()
{
    return getWorldTransform().getAxisY().normalized();
}

Vector SceneComponent::getRightDirection()
{
    return getWorldTransform().getAxisX().normalized();
}

Vector SceneComponent::getUpDirection()
{
    return getWorldTransform().getAxisZ().normalized();
}

void SceneComponent::markLocalTransformDirty()
{
    if ( !m_dirtyLocal )
    {
        m_dirtyLocal = true;

        if ( !m_dirtyWorld )
        {
            markWorldTransformDirty();
        }
    }
}

void SceneComponent::markWorldTransformDirty()
{
    if ( !m_dirtyWorld )
    {
        m_dirtyWorld = true;

        for ( auto& child : m_children )
        {
            if ( !child->m_dirtyWorld )
            {
                child->markWorldTransformDirty();
            }
        }
    }
}

void SceneComponent::recalculateMatrices()
{
    if ( m_dirtyLocal )
    {
        m_localMatrix = TransformMatrix( m_localPosition, m_localRotation, m_localScale );
        m_dirtyLocal = false;
    }

    if ( m_dirtyWorld )
    {
        if ( !m_parent )
        {
            m_localToWorldMatrix = m_localMatrix;
        }
        else
        {
            m_localToWorldMatrix = m_parent->getWorldTransform() * m_localMatrix;
        }

        m_worldToLocalMatrix = m_localToWorldMatrix.inverse();

		m_worldMatrixInverseTranspose = m_localToWorldMatrix.inverse().transposed();
        m_worldMatrixInverseTranspose[0][3] = 0.0f;
        m_worldMatrixInverseTranspose[1][3] = 0.0f;
        m_worldMatrixInverseTranspose[2][3] = 0.0f;
        m_worldMatrixInverseTranspose[3][3] = 1.0f;
        m_worldMatrixInverseTranspose[3][0] = 0.0f;
        m_worldMatrixInverseTranspose[3][1] = 0.0f;
        m_worldMatrixInverseTranspose[3][2] = 0.0f;

        m_dirtyWorld = false;
    }
}

