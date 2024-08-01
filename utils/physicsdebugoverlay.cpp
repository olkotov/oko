// Oleg Kotov

#include "physicsdebugoverlay.h"
#include "debugoverlay.h"
#include "okoengine.h"
#include "log.h"

void PhysicsDebugOverlay::drawLine( const btVector3& from, const btVector3& to, const btVector3& color )
{
	engine()->debugOverlay()->addLine(
		Vector( from.x(), from.y(), from.z() ),
		Vector( to.x(), to.y(), to.z() ),
		Color( color.x() * 255, color.y() * 255, color.z() * 255 )
	);
}

void PhysicsDebugOverlay::drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color )
{
	// currently not implemented
}

void PhysicsDebugOverlay::reportErrorWarning( const char* warningString )
{
	Warning( "[BulletPhysics]: %s", warningString );
}

void PhysicsDebugOverlay::draw3dText( const btVector3& location, const char* textString )
{
	// currently not implemented
}

