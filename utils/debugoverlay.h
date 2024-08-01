// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "vertex.h"
#include "vector.h"
#include "color.h"
#include "matrix.h"

#include <stdint.h>
#include <vector>

class CameraComponent;
class Shader;

struct Skeleton;
struct SkeletalJoint;

struct ENGINE_API DebugLine
{
	DebugLine( const Vector& start, const Vector& end, const Color& color )
		: m_start( start )
		, m_end( end )
		, m_color( color )
	{}

	Vector m_start;
	Vector m_end;
	Color  m_color;
};

class ENGINE_API DebugOverlay
{
public:
       
	DebugOverlay();
	~DebugOverlay();

	void setDrawEnabled( bool state );
	bool isDrawEnabled() const;

    void addLine( const Vector& start, const Vector& end, const Color& color = Color::White /*, float lineWidth = 1.0f, float duration = 0.0f, bool depthEnabled = true */ );
    void addBox( const Matrix& transform, const Color& color = Color::White );
	void addCircle( const Vector& centerPosition, const Vector& planeNormal, float radius, const Color& color = Color::White /* , float duration = 0.0f, bool depthEnabled = true */ );
	void addSphere( const Vector& centerPosition, float radius, const Color& color = Color::White /* , float duration = 0.0f, bool depthEnabled = true */ );
	// void addAABB( const Point& minCoords, const Point& maxCoords, Color color, float lineWidth = 1.0f, float duration = 0.0f, bool depthEnabled = true );
	// void addOBB( const Mat44& centerTransform, const Vector& scaleXYZ, Color color, float lineWidth = 1.0f, float duration = 0.0f, bool depthEnabled = true );
	// void addText( const Point& pos, const char* text, Color color, float duration = 0.0f, bool depthEnabled = true );
    void addCameraFrustum( CameraComponent* camera, const Color& color = Color::White );
    void addSkeleton( Skeleton* skeleton, const Matrix& localToWorld = Matrix::Identity );
	void addTransformAxis( const Matrix& transform );
	// addCross()

    void draw();

private:

	void addSkeletalBones( SkeletalJoint* joint, const Matrix& localToWorld );

	void flushVertexData() const;
	void bindVertexDataToShader( Shader* shader ) const;

	void prepareData();
	void cleanup();

private:

	// 
	bool m_enabled = false;

	// Stores all lines.
	std::vector<DebugLine> m_lines;

	// Stores all vertices.
	std::vector<DebugVertex> m_vertices; // with depth test and without, tmp when render

	// The vertex array object handle.
	uint32_t m_vertexArray = 0;

	// The vertex buffer handle.
	uint32_t m_vertexBuffer = 0;

	// The size of the allocated memory for the vertex buffer.
	int32_t m_vertexBufferAllocSize = 0;

	//
	Shader* m_debugShader = nullptr;
};

