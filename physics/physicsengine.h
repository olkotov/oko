// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "hitresult.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <stdint.h>
#include <set>
#include <map>
#include <vector>

class PrimitiveComponent;
class PhysicsDebugOverlay;

typedef std::pair<PrimitiveComponent*, PrimitiveComponent*> CollisionPair;
typedef std::set<CollisionPair> CollisionPairSet;

/// Update Physics World, Sync Physics and Game Engine, handle ray and shape casts.
class ENGINE_API Physics
{
public:
    
    Physics();
    ~Physics();
  
	void update( float deltaTime );

    void simulate( float deltaTime );

    void updateKinematic() const;
    void updateDynamic() const;

    static void checkExitEvents();
    static void resetCollisionEvents();

    static void collisionCallback( btDynamicsWorld* dynamicsWorld, btScalar timeStep );
    static void handleCollisionEvents( btPersistentManifold* manifold );

    // btRigidBody CreateRigidBody( float mass, btCollisionShape* shape, btTransform transform );

    bool raycast( HitResult& hitResult, const Vector& source, const Vector& target /*, IntersectOptions options, Entity ^ignoreEntity*/ );
    bool raycastMultiple( std::vector<HitResult>& hitResults, const Vector& start, const Vector& end );
    // void Raycast( const CVector& source, const CVector& direction, float maxDistance /*, IntersectOptions options, Entity ^ignoreEntity*/ );
    bool sphereCast( HitResult& hitResult, const Vector& source, const Vector& target, float radius );

    btDiscreteDynamicsWorld* getBulletPhysicsWorld() const
    {
        return m_physicsWorld;
    }

    void setGravity( const Vector& gravity );
    Vector getGravity() const;

    void queryObjectsInSphere( std::vector<class RigidBody*>& result, const Vector& center, float radius ) const;

    void removeComponentFromSystem( PrimitiveComponent* component );

private:

	// std::vector<PrimitiveComponent*> _primitives;

    // Bullet collision configuration.
    btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;

    // Bullet collision dispatcher.
    btCollisionDispatcher* m_dispatcher = nullptr;

    // Bullet collision broadphase.
    btBroadphaseInterface* m_overlappingPairCache = nullptr;

    // Bullet constraint solver.
    btSequentialImpulseConstraintSolver* m_constraintSolver = nullptr;

    // Bullet physics world.
    btDiscreteDynamicsWorld* m_physicsWorld = nullptr;

    // 
    PhysicsDebugOverlay* m_physicsDebugOverlay = nullptr;

    // avoid of double event dispatching
    // ( a <-> b, a <-> b == sort( b <-> a ) )
	static std::map<CollisionPair, bool> m_frameCollisions; // tickCollisions?

	static std::set<CollisionPair> m_pairsPrevUpdate;
	static std::set<CollisionPair> m_pairsThisUpdate;
};

/*


	btDefaultCollisionConfiguration
	btCollisionDispatcher
	btBroadphaseInterface
	btSequentialImpulseConstraintSolver
	btDiscreteDynamicsWorld

*/

