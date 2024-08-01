// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "entitycomponent.h"
#include "entityfactory.h"

#include <typeindex>

class Level;
class SceneComponent;
class PrimitiveComponent;
struct HitResult;

class ENGINE_API Entity
{
    friend class Level;

public:

    virtual ~Entity();

	void setName( const std::string& name );
    const std::string& getName() const;

    void setEnabled( bool enabled )
	{
		m_enabled = enabled;
	}

	bool isEnabled() const
	{
		return m_enabled;
	}

    bool isA( const char* classname )
    {
        return !strcmp( getClassname().c_str(), classname );
    }

    const std::string& getClassname() const;

	template <class ComponentType>
	ComponentType* createComponent( const std::string& name = "" )
	{
		ComponentType* component = new ComponentType();

		if ( name.empty() != true )
		{
			EntityComponent* entityComponent = static_cast<EntityComponent*>( component );
			entityComponent->setName( name );
		}

		addComponent( component );

		return static_cast<ComponentType*>( component );
	}

    // unique instance check
	void addComponent( EntityComponent* component );
    // removeComponent

    template <class Type>
    Type* getComponent()
    {
        for ( const auto& component : m_components )
        {
            // if ( typeid( *component ) == typeid( Type ) )
            if ( std::type_index( typeid( *component ) ) == std::type_index( typeid( Type ) ) )
            {
                return dynamic_cast<Type*>( component );
            }
        }

        return nullptr;
    }

    const std::vector<EntityComponent*>& getComponents() const;
    
    virtual void beginPlay();
    virtual void tick( float deltaTime );
    virtual void endPlay();

    void destroy();

    // load some assets
    virtual void precache() {}

    virtual bool IsPlayer() const { return false; }
    
    virtual bool isInteractable() const { return false; }
    virtual void interact() {}

    virtual void hit() {}

	void setRootComponent( SceneComponent* component )
	{
        m_rootComponent = component;
    }

    SceneComponent* getRootComponent() const
    {
        return m_rootComponent;
    }

    const Matrix& getWorldTransform();

    // transform

    void setWorldPosition( const Vector& location );
    Vector getWorldPosition() const;

    void translate( const Vector& deltaLocation );
    void translateLocal( const Vector& deltaLocation ) const;

    void setWorldEulerAngles( const Vector& angles );
    void setWorldRotation( const Quaternion& rotation );

    Vector getWorldEulerAngles() const;
    Quaternion getWorldRotation() const;

    void setRelativePosition( const Vector& position ) const;
    void setRelativeEulerAngles( const Vector& angles ) const;
    void setRelativeScale( const Vector& scale ) const;

    Vector getRelativeEulerAngles() const;

    void rotate( const Vector& deltaAngles );
    void rotate( const Quaternion& delta );
    void rotateLocal( const Vector& deltaAngles );
    void rotateLocal( const Quaternion& delta );
    
    Vector getForwardDirection();
    Vector getRightDirection();
    Vector getUpDirection();
    
    // physics

    void addForce( const Vector& force ) const;
    void addForceAtLocation( const Vector& force, const Vector& location ) const;

    void applyImpulse( const Vector& impulse ) const;
    void addImpulseAtLocation( const Vector& impulse, const Vector& location ) const;

    void setLinearVelocity( const Vector& velocity ) const;
    Vector getLinearVelocity() const;

    void setAngularVelocity( const Vector& velocity ) const;
    Vector getAngularVelocity() const;

    virtual void onOverlapBegin( PrimitiveComponent* overlappedComponent, Entity* otherEntity, PrimitiveComponent* otherComponent ) {}
    virtual void onOverlapStay( PrimitiveComponent* overlappedComponent, Entity* otherEntity, PrimitiveComponent* otherComponent ) {}
    virtual void onOverlapEnd( PrimitiveComponent* overlappedComponent, Entity* otherEntity, PrimitiveComponent* otherComponent ) {}

    virtual void onCollisionBegin( PrimitiveComponent* overlappedComponent, Entity* otherEntity, PrimitiveComponent* otherComponent, const HitResult& hit ) {}
    virtual void onCollisionStay( PrimitiveComponent* overlappedComponent, Entity* otherEntity, PrimitiveComponent* otherComponent ) {}
    virtual void onCollisionEnd( PrimitiveComponent* overlappedComponent, Entity* otherEntity, PrimitiveComponent* otherComponent ) {}
	
    virtual void notifyHit( PrimitiveComponent* overlappedComponent, Entity* otherEntity, PrimitiveComponent* otherComponent, const Vector& hitPosition, const Vector& hitNormal /* impulse */ ) {}

    virtual void onResolutionChanged( uint16_t width, uint16_t height ) {};

    // ---

    Level* getWorld() const
    {
        return m_world;
    }

    bool isInWorld() const
    {
        return ( m_world ) ? true : false;
    }

    bool isPendingKill() const
    {
        return m_pendingKill;
    }

public:

    std::string m_classname;

private:

	std::string m_name;
    bool m_enabled = true;
	bool m_pendingKill = false;
	SceneComponent* m_rootComponent = nullptr;
	std::vector<EntityComponent*> m_components;
    Level* m_world = nullptr;
};

#define LINK_ENTITY_TO_CLASS( classname, class ) \
static EntityFactory<class> classname( #classname );

inline bool classnameIs( Entity* entity, const char* classname )
{
	assert( entity );
	return entity && !strcmp( entity->getClassname().c_str(), classname );
}

