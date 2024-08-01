// Oleg Kotov

#pragma once

#include <bullet/LinearMath/btIDebugDraw.h>

class PhysicsDebugOverlay : public btIDebugDraw
{
public:

	virtual void drawLine( const btVector3& from, const btVector3& to, const btVector3& color ) override;
	virtual void drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color ) override;

	virtual void reportErrorWarning( const char* warningString ) override;

	virtual void draw3dText( const btVector3& location, const char* textString ) override;

	virtual void setDebugMode( int debugMode ) override { m_debugDrawMode = debugMode; }
	virtual int getDebugMode() const override { return m_debugDrawMode; }

private:

	int m_debugDrawMode = 0;
};

