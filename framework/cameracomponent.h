// Oleg Kotov

#pragma once

#include "scenecomponent.h"

class ENGINE_API CameraComponent : public SceneComponent
{
public:

	enum class ProjectionMode : uint8_t
	{
		Perspective,
		Orthographic
	};

	struct CameraViewport
	{
		float x = 0.0f;      // 0-1
		float y = 0.0f;      // 0-1
		float width = 1.0f;  // 0-1
		float height = 1.0f; // 0-1
	};

public:
    
	CameraComponent();
    ~CameraComponent();

    void recalculateMatrices();

public:
	
	void activate();

	Vector worldToScreen( const Vector& worldCoord ); // world3dTo
	Vector screenToWorld( int32_t screen_x, int32_t screen_y, int32_t depth );

	//
	void setProjectionMode( ProjectionMode mode );

	//
	CameraComponent::ProjectionMode getProjectionMode() const;

	//
	const Matrix& getProjectionMatrix();

	//
	void updateProjectionMatrix();

	//
	const Matrix& getViewMatrix() const;

	//
	const Matrix& getViewProjectionMatrix() const;

	//
	const Matrix& getNormalMatrix() const;

	//
	const CameraComponent::CameraViewport& getViewport() const;

	// Convert a point from 3D world space to 2D canvas pixel space.

	/**
	 * Sets a vertical field of view of the camera and updates the projection matrix.
	 *
	 * @param fov  The vertical field of view in degrees.
	 */
	void setFov( float fov );

	// Returns the current vertical field of view of the camera.
	float getFov() const;

	//
	void setAspectRatio( float aspect );

	//
	float getAspectRatio() const;
	
	//
	void setNearClip( float nearClip );

	//
	float getNearClip() const;

	//
	void setFarClip( float farClip );

	//
	float getFarClip() const;

	//
	void setBackgroundColor( const Color& color );

	//
	const Color& getBackgroundColor() const;

	void setOrthographicSize( float size )
	{
		m_orthographicSize = size;
		m_projectionMatrixDirty = true;
	}

	float getOrthographicSize() const
	{
		return m_orthographicSize;
	}

	// frustumToSphereCollisionTest()
	// bool isSphereVisible( const Vector& position, float radius ) const;

private:

	//
	CameraComponent::ProjectionMode m_projectionMode = CameraComponent::ProjectionMode::Perspective;

	//
	Matrix m_projectionMatrix = Matrix::Identity;
	
	//
	Matrix m_viewMatrix = Matrix::Identity;           // worldToCameraMatrix
	
	//
	Matrix m_viewProjectionMatrix = Matrix::Identity; // cullingMatrix

	// 
	Matrix m_normalMatrix = Matrix::Identity;

	//
	CameraComponent::CameraViewport m_viewport;

	//
	float m_orthographicSize = 5.0f;

	// The vertical field of view of the camera, in radians.
	float m_fov = 70.0f * Deg2Rad;   // Vertical fov in radians [0..1 * 2PI].

	//
	float m_aspectRatio = 16.0f / 9.0f;

	//
	float m_nearClip = 0.1f;

	//
	float m_farClip = 1000.0f;

	//
	Color m_backgroundColor = Color::Black;

	// Dirty flag of the projection matrix.
	bool m_projectionMatrixDirty = true;
};

