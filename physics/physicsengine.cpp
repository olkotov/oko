// Oleg Kotov

#include "physicsengine.h"
#include "primitivecomponent.h"
#include "entity.h"
#include "physicsdebugoverlay.h"
#include "log.h"
#include "okoengine.h"
#include "debugoverlay.h"

#include <iterator>
#include <algorithm>

std::map<CollisionPair, bool> Physics::m_frameCollisions;

std::set<CollisionPair> Physics::m_pairsPrevUpdate;
std::set<CollisionPair> Physics::m_pairsThisUpdate;

Physics::Physics()
{
	Msg( "physics:initialize" );

	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher( m_collisionConfiguration );
	m_overlappingPairCache = new btDbvtBroadphase();
	m_constraintSolver = new btSequentialImpulseConstraintSolver();
	m_physicsWorld = new btDiscreteDynamicsWorld( m_dispatcher, m_overlappingPairCache, m_constraintSolver, m_collisionConfiguration );
	
    m_physicsWorld->setGravity( btVector3( 0.0f, 0.0f, -9.81f ) );
    m_physicsWorld->getDispatchInfo().m_useContinuous = true;
    m_physicsWorld->getSolverInfo().m_splitImpulse = false; // disable by default for performance
    m_physicsWorld->setInternalTickCallback( collisionCallback );
    m_physicsWorld->setSynchronizeAllMotionStates( true );

    // debug draw

    m_physicsDebugOverlay = new PhysicsDebugOverlay();
    m_physicsDebugOverlay->setDebugMode( btIDebugDraw::DBG_DrawWireframe );

    m_physicsWorld->setDebugDrawer( m_physicsDebugOverlay );
}

Physics::~Physics()
{
	Msg( "physics:shutdown" );

	SafeDelete( m_physicsWorld );
	SafeDelete( m_constraintSolver );
	SafeDelete( m_overlappingPairCache );
	SafeDelete( m_dispatcher );
	SafeDelete( m_collisionConfiguration );
}

void Physics::update( float deltaTime )
{
    simulate( deltaTime );
}

void Physics::collisionCallback( btDynamicsWorld* dynamicsWorld, btScalar timeStep )
{
    // overlappingPairArray is created in the broadphase while manifold is created in the narrowphase

    btDispatcher* dispatcher = dynamicsWorld->getDispatcher();
    int numManifolds = dispatcher->getNumManifolds();

    for ( int32_t i = 0; i < numManifolds; ++i )
    {
        btPersistentManifold* manifold = dispatcher->getManifoldByIndexInternal( i );
        handleCollisionEvents( manifold );
    }

	checkExitEvents();
	resetCollisionEvents();
}

void Physics::handleCollisionEvents( btPersistentManifold* manifold )
{
    if ( manifold->getNumContacts() == 0 ) return;

    // get bodies from manifold
    const btCollisionObject* body0 = manifold->getBody0();
    const btCollisionObject* body1 = manifold->getBody1();
    
    // get components from bodies
    PrimitiveComponent* component0 = static_cast<PrimitiveComponent*>( body0->getUserPointer() );
    PrimitiveComponent* component1 = static_cast<PrimitiveComponent*>( body1->getUserPointer() );
    
    // double check
    assert( component0 != nullptr );
    assert( component1 != nullptr );
    
    // component0 and component1 is triggers
    // we not handle events for this
    if ( !component0->getCollisionEnabled() && !component1->getCollisionEnabled() ) return;

    // sort by pointer value
    if ( component0 > component1 )
    {
        std::swap( component0, component1 );
    }

    // create the collision pair
    CollisionPair collisionPair = std::make_pair( component0, component1 );

    m_pairsThisUpdate.insert( collisionPair );

    // single call callback on frame for collisionPair
	if ( m_frameCollisions.find( collisionPair ) != m_frameCollisions.end() ) return;

    // insert the pair into the list
    m_frameCollisions[collisionPair] = true;

    // ----

    int32_t numContacts = manifold->getNumContacts();
    
    for ( int32_t i = 0; i < numContacts; ++i )
    {
        btManifoldPoint& contactPoint = manifold->getContactPoint( i );
        if ( contactPoint.getDistance() >= 0.0f ) continue;

        const btVector3& normalWorldOnB = contactPoint.m_normalWorldOnB;

        // begin event
        if ( m_pairsPrevUpdate.find( collisionPair ) == m_pairsPrevUpdate.end() )
        {
            // collision event
            if ( component0->getCollisionEnabled() && component1->getCollisionEnabled() )
            {
                {
                    PrimitiveComponent* collisionComponent = component0;
				    Entity* otherEntity = component1->getOwner();
				    PrimitiveComponent* otherComponent = component1;

                    HitResult hit;
                    hit.normal = Vector( normalWorldOnB.x(), normalWorldOnB.y(), normalWorldOnB.z() );

				    component0->getOwner()->onCollisionBegin( collisionComponent, otherEntity, otherComponent, hit );
                }

                {
                    PrimitiveComponent* collisionComponent = component1;
				    Entity* otherEntity = component0->getOwner();
				    PrimitiveComponent* otherComponent = component0;

                    HitResult hit;
                    hit.normal = Vector( normalWorldOnB.x(), normalWorldOnB.y(), normalWorldOnB.z() );

				    component1->getOwner()->onCollisionBegin( collisionComponent, otherEntity, otherComponent, hit );
                }
            }
            // overlap event
            else
            {
                {
                    PrimitiveComponent* overlappedComponent = component0;
				    Entity* otherEntity = component1->getOwner();
				    PrimitiveComponent* otherComponent = component1;

				    component0->getOwner()->onOverlapBegin( overlappedComponent, otherEntity, otherComponent );
                }

                {
                    PrimitiveComponent* overlappedComponent = component1;
				    Entity* otherEntity = component0->getOwner();
				    PrimitiveComponent* otherComponent = component0;

				    component1->getOwner()->onOverlapBegin( overlappedComponent, otherEntity, otherComponent );
                }
            }
        }
        // stay event
        else
        {
            // collision event
            if ( component0->getCollisionEnabled() && component1->getCollisionEnabled() )
            {
                {
                    PrimitiveComponent* collisionComponent = component0;
				    Entity* otherEntity = component1->getOwner();
				    PrimitiveComponent* otherComponent = component1;

				    component0->getOwner()->onCollisionStay( collisionComponent, otherEntity, otherComponent );
                }

                {
                    PrimitiveComponent* collisionComponent = component1;
				    Entity* otherEntity = component0->getOwner();
				    PrimitiveComponent* otherComponent = component0;

				    component1->getOwner()->onCollisionStay( collisionComponent, otherEntity, otherComponent );
                }
            }
            // overlap event
            else
            {
                {
                    PrimitiveComponent* overlappedComponent = component0;
				    Entity* otherEntity = component1->getOwner();
				    PrimitiveComponent* otherComponent = component1;

				    component0->getOwner()->onOverlapStay( overlappedComponent, otherEntity, otherComponent );
                }

                {
                    PrimitiveComponent* overlappedComponent = component1;
				    Entity* otherEntity = component0->getOwner();
				    PrimitiveComponent* otherComponent = component0;

				    component1->getOwner()->onOverlapStay( overlappedComponent, otherEntity, otherComponent );
                }
            }
        }
    }
}

void Physics::updateDynamic() const
{
	btAlignedObjectArray<btRigidBody*> bodies = m_physicsWorld->getNonStaticRigidBodies();

	for ( int32_t i = 0; i < bodies.size(); ++i )
	{
        if ( bodies[i]->isStaticOrKinematicObject() )
        {
            continue;
        }

        if ( !bodies[i]->isActive() )
        {
            continue;
        }

        const btMotionState* motionState = bodies[i]->getMotionState();

        if ( !motionState )
        {
            continue;
        }

        btTransform worldTransform;
        motionState->getWorldTransform( worldTransform );

        // btTransform& worldTransform = bodies[i]->getWorldTransform();

        btVector3 p = worldTransform.getOrigin();
        btQuaternion q = worldTransform.getRotation();

		PrimitiveComponent* primitive = static_cast<PrimitiveComponent*>( bodies[i]->getUserPointer() );
        assert( primitive != nullptr );
 
        primitive->setWorldPosition( Vector( p.x(), p.y(), p.z() ), false );
        primitive->setWorldRotation( Quaternion( q.x(), q.y(), q.z(), q.w() ), false );
	}
}

void Physics::checkExitEvents()
{
    std::set<CollisionPair> removedPairs;
    
    std::set_difference(
        m_pairsPrevUpdate.begin(), m_pairsPrevUpdate.end(),
        m_pairsThisUpdate.begin(), m_pairsThisUpdate.end(),
        std::inserter( removedPairs, removedPairs.begin() )
    );
    
    for ( const auto& collisionPair : removedPairs )
    {
        PrimitiveComponent* component0 = collisionPair.first;
        PrimitiveComponent* component1 = collisionPair.second;

        // collision event
        if ( component0->getCollisionEnabled() && component1->getCollisionEnabled() )
        {
            {
                PrimitiveComponent* collisionComponent = component0;
				Entity* otherEntity = component1->getOwner();
				PrimitiveComponent* otherComponent = component1;

				component0->getOwner()->onCollisionEnd( collisionComponent, otherEntity, otherComponent );
            }

            {
                PrimitiveComponent* collisionComponent = component1;
				Entity* otherEntity = component0->getOwner();
				PrimitiveComponent* otherComponent = component0;

				component1->getOwner()->onCollisionEnd( collisionComponent, otherEntity, otherComponent );
            }
        }
        // overlap event
        else
        {
            {
                PrimitiveComponent* overlappedComponent = component0;
				Entity* otherEntity = component1->getOwner();
				PrimitiveComponent* otherComponent = component1;

				component0->getOwner()->onOverlapEnd( overlappedComponent, otherEntity, otherComponent );
            }

            {
                PrimitiveComponent* overlappedComponent = component1;
				Entity* otherEntity = component0->getOwner();
				PrimitiveComponent* otherComponent = component0;

				component1->getOwner()->onOverlapEnd( overlappedComponent, otherEntity, otherComponent );
            }
        }
    }
}

void Physics::resetCollisionEvents()
{
    m_pairsPrevUpdate = m_pairsThisUpdate;
    m_pairsThisUpdate.clear();
    m_frameCollisions.clear();
}

void Physics::updateKinematic() const
{
    btAlignedObjectArray<btRigidBody*> bodies = m_physicsWorld->getNonStaticRigidBodies();

	for ( int32_t i = 0; i < bodies.size(); ++i )
	{
        if ( bodies[i]->isStaticOrKinematicObject() == true )
        {
            btMotionState* motionState = bodies[i]->getMotionState();

            if ( motionState != nullptr )
            {
                PrimitiveComponent* primitive = static_cast<PrimitiveComponent*>( bodies[i]->getUserPointer() );
                assert( primitive != nullptr );

                btTransform worldTransform;

                Vector p = primitive->getWorldPosition();
                Quaternion q = primitive->getWorldRotation();

                worldTransform.setOrigin( btVector3( p.x, p.y, p.z ) );
                worldTransform.setRotation( btQuaternion( q.x, q.y, q.z, q.w ) );

                motionState->setWorldTransform( worldTransform );
            }
        }
	}
}

void Physics::simulate( float deltaTime )
{
    updateKinematic();

    // step the physics simulation
    m_physicsWorld->stepSimulation( deltaTime );

    updateDynamic();

    if ( engine()->debugOverlay()->isDrawEnabled() )
    {
        m_physicsWorld->debugDrawWorld();
    }
}

bool Physics::raycast( HitResult& hit, const Vector& start, const Vector& end )
{
    btVector3 rayOrigin( start.x, start.y, start.z );
    btVector3 rayTarget( end.x, end.y, end.z );

    btCollisionWorld::ClosestRayResultCallback resultCallback( rayOrigin, rayTarget );
    m_physicsWorld->rayTest( rayOrigin, rayTarget, resultCallback );

    if ( resultCallback.hasHit() )
    {
        btVector3 point = resultCallback.m_hitPointWorld;
        hit.position = Vector( point.x(), point.y(), point.z() );

        btVector3 normal = resultCallback.m_hitNormalWorld;
        hit.normal = Vector( normal.x(), normal.y(), normal.z() );

        hit.component = static_cast<PrimitiveComponent*>( resultCallback.m_collisionObject->getUserPointer() );
        hit.entity = hit.component->getOwner();

        // calc it in game code, performance reason?
        hit.distance = ( hit.position - start ).length();

        return true;
    }

    return false;
}

bool Physics::raycastMultiple( std::vector<HitResult>& hits, const Vector& start, const Vector& end )
{
	btVector3 rayOrigin( start.x, start.y, start.z );
	btVector3 rayTarget( end.x, end.y, end.z );

	btCollisionWorld::AllHitsRayResultCallback resultCallback( rayOrigin, rayTarget );
	m_physicsWorld->rayTest( rayOrigin, rayTarget, resultCallback );

	if ( resultCallback.hasHit() )
	{
		const int numHits = resultCallback.m_collisionObjects.size();
		hits.reserve( numHits );

		for ( int i = 0; i < numHits; ++i )
		{
			HitResult hit;

			btVector3 point = resultCallback.m_hitPointWorld[i];
			hit.position = Vector( point.x(), point.y(), point.z() );

			btVector3 normal = resultCallback.m_hitNormalWorld[i];
			hit.normal = Vector( normal.x(), normal.y(), normal.z() );

			hit.component = static_cast<PrimitiveComponent*>( resultCallback.m_collisionObjects[i]->getUserPointer() );
			hit.entity = hit.component->getOwner();

			// calc it in game code, performance reason?
			hit.distance = ( hit.position - start ).length();

			hits.push_back( hit );
		}

		return true;
	}

	return false;
}

bool Physics::sphereCast( HitResult& hitResult, const Vector& source, const Vector& target, float radius )
{
    btSphereShape shape( radius );

    btVector3 castOrigin( source.x, source.y, source.z );
    btVector3 castTarget( target.x, target.y, target.z );

    btCollisionWorld::ClosestConvexResultCallback resultCallback( castOrigin, castTarget );

    m_physicsWorld->convexSweepTest(
        &shape,
        btTransform( btQuaternion::getIdentity(), resultCallback.m_convexFromWorld ),
        btTransform( btQuaternion::getIdentity(), resultCallback.m_convexToWorld ),
        resultCallback
    );

    if ( resultCallback.hasHit() )
    {
        hitResult.distance = ( target - source ).length();

        btVector3 point = resultCallback.m_hitPointWorld;
        hitResult.position = Vector( point.x(), point.y(), point.z() );

        btVector3 normal = resultCallback.m_hitNormalWorld;
        hitResult.normal = Vector( normal.x(), normal.y(), normal.z() );

        hitResult.component = static_cast<PrimitiveComponent*>( resultCallback.m_hitCollisionObject->getUserPointer() );
        hitResult.entity = hitResult.component->getOwner();

        return true;
    }

    return false;
}

void Physics::setGravity( const Vector& gravity )
{
    m_physicsWorld->setGravity( btVector3( gravity.x, gravity.y, gravity.z ) );
}

Vector Physics::getGravity() const
{
	btVector3 g = m_physicsWorld->getGravity();
	return Vector( g.x(), g.y(), g.z() );
}

void Physics::queryObjectsInSphere( std::vector<class RigidBody*>& result, const Vector& center, float radius ) const
{
    // result.clear();

    btSphereShape sphereShape( radius );
    btRigidBody* tempRigidBody( new btRigidBody( 1.0f, nullptr, &sphereShape ) );
    tempRigidBody->setWorldTransform( btTransform( btQuaternion::getIdentity(), btVector3( center.x, center.y, center.z ) ) );

    // need to activate the temporary rigid body to get reliable results from static, sleeping objects
    tempRigidBody->activate();
        
    m_physicsWorld->addRigidBody( tempRigidBody );

    // PhysicsQueryCallback callback( result, collisionMask );
    // world_->contactTest( tempRigidBody.get(), callback );

    m_physicsWorld->removeRigidBody( tempRigidBody );
}

void Physics::removeComponentFromSystem( PrimitiveComponent* component )
{
    // m_frameCollisions
    {
        auto it = m_frameCollisions.begin();

        while ( it != m_frameCollisions.end() )
        {
            auto pair = it->first;

            if ( pair.first == component || pair.second == component )
            {
                it = m_frameCollisions.erase( it );
            }
            else
            {
                ++it;
            }
        }
    }

    // m_pairsPrevUpdate
    {
        auto it = m_pairsPrevUpdate.begin();

        while ( it != m_pairsPrevUpdate.end() )
        {
            if ( it->first == component || it->second == component )
            {
                it = m_pairsPrevUpdate.erase( it );
            }
            else
            {
                ++it;
            }
        }
    }

    // m_pairsThisUpdate
    {
        auto it = m_pairsThisUpdate.begin();

        while ( it != m_pairsThisUpdate.end() )
        {
            if ( it->first == component || it->second == component )
            {
                it = m_pairsThisUpdate.erase( it );
            }
            else
            {
                ++it;
            }
        }
    }
}

/*

#include <btBulletDynamicsCommon.h>

int main() {

    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10, 0));


    btTriangleMesh* triangleMesh = new btTriangleMesh();

    triangleMesh->addTriangle(btVector3(0, 0, 0), btVector3(1, 0, 0), btVector3(0, 1, 0), true);
    triangleMesh->addTriangle(btVector3(1, 0, 0), btVector3(1, 1, 0), btVector3(0, 1, 0), true);

    btBvhTriangleMeshShape* triangleMeshShape = new btBvhTriangleMeshShape(triangleMesh, true);


    btCollisionObject* triangleMeshObject = new btCollisionObject();
    triangleMeshObject->setCollisionShape(triangleMeshShape);
    triangleMeshObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    dynamicsWorld->addCollisionObject(triangleMeshObject);


    btCollisionShape* sphereShape = new btSphereShape(1);
    btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
    btScalar sphereMass = 1;
    btVector3 sphereInertia(0, 0, 0);
    sphereShape->calculateLocalInertia(sphereMass, sphereInertia);
    btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(sphereMass, sphereMotionState, sphereShape, sphereInertia);
    btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
    dynamicsWorld->addRigidBody(sphereRigidBody);


    btCollisionWorld::ContactResultCallback resultCallback;
    dynamicsWorld->contactPairTest(triangleMeshObject, sphereRigidBody, resultCallback);

    for (int i = 0; i < resultCallback.m_manifoldPtr->getNumContacts(); i++) {
        btManifoldPoint& pt = resultCallback.m_manifoldPtr->getContactPoint(i);
        btVector3 ptA = pt.getPositionWorldOnA();
        btVector3 ptB = pt.getPositionWorldOnB();
        btVector3 normalOnB = pt.m_normalWorldOnB;

    }


    delete sphereRigidBody;
    delete sphereShape;
    delete triangleMeshObject;
    delete triangleMeshShape;
    delete triangleMesh;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;

    return 0;
}

*/

