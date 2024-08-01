// Oleg Kotov

#include "cameracomponent.h"
#include "perspectivematrix.h"
#include "orthomatrix.h"
#include "lookatmatrix.h"
#include "okoengine.h"
#include "graphics.h"
#include "window.h"

CameraComponent::CameraComponent()
{
    m_aspectRatio = engine()->window()->getAspectRatio();

	if ( engine()->graphics()->getActiveCamera() == nullptr )
	{
        engine()->graphics()->setActiveCamera( this );
	}
}

CameraComponent::~CameraComponent()
{
	if ( engine()->graphics()->getActiveCamera() == this )
	{
		engine()->graphics()->setActiveCamera( nullptr );
	}
}

void CameraComponent::recalculateMatrices()
{
    // just trigger update if dirty
    getProjectionMatrix();

    // TODO: something like dirty flag for view matrix?

    Vector location = getWorldPosition();
    Vector forwardDir = getForwardDirection();
    Vector target = location + forwardDir;
    
    // viewMatrix = inverse( cameraMatrix )
    // cameraMatrix = camera.getWorldTransform()
    m_viewMatrix = LookAtMatrix( location, target, getUpDirection() );
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

    // !!!
    // normalMatrix is transpose( inverse( modelViewMatrix ) ), modelMatrix of mesh!
    // _worldInverseTranspose 
    // mat3 normalMatrix = mat3( transpose( inverse( modelViewMatrix ) ) );
    m_normalMatrix = getWorldTransform().inverse().transposed();
    m_normalMatrix[0][3] = 0.0f;
    m_normalMatrix[1][3] = 0.0f;
    m_normalMatrix[2][3] = 0.0f;
    m_normalMatrix[3][3] = 1.0f;
    m_normalMatrix[3][0] = 0.0f;
    m_normalMatrix[3][1] = 0.0f;
    m_normalMatrix[3][2] = 0.0f;
}

void CameraComponent::activate()
{
    engine()->graphics()->setActiveCamera( this );
}

Vector CameraComponent::worldToScreen( const Vector& worldCoord )
{
    uint16_t width, height;
    engine()->window()->getResolution( width, height );

    // update viewProjectionMatrix
    recalculateMatrices();
   
	// world space ---> clip space

    Vector screenCoord = m_viewProjectionMatrix.transformPoint( worldCoord );

    // calculate w co-coord
    float w = worldCoord.x * m_viewProjectionMatrix[3][0] +
              worldCoord.y * m_viewProjectionMatrix[3][1] +
              worldCoord.z * m_viewProjectionMatrix[3][2] +
                         1 * m_viewProjectionMatrix[3][3];

	// clip space ---> normalized device space

    screenCoord.x /= w;
    screenCoord.y /= w;

	// normalized device space ---> window space

    screenCoord.x = ( screenCoord.x * 0.5f + 0.5f ) * width;
    screenCoord.y = ( 0.5f - screenCoord.y * 0.5f ) * height;
    screenCoord.z = w;
    
    return screenCoord;
}

void getPerspectiveHalfSize( Vector& halfSize, float fov, float aspect, float znear )
{
    fov *= Rad2Deg;
    
    halfSize.y = znear * tanf( fov * PI / 360.0f );
    halfSize.x = halfSize.y * aspect;
}

Vector CameraComponent::screenToWorld( int32_t screen_x, int32_t screen_y, int32_t depth )
{
    Vector worldCoord;

    int32_t width = engine()->graphics()->getWindowWidth();
    int32_t height = engine()->graphics()->getWindowHeight();

    // calculate the screen click as a point on the far plane of the normalized device coordinate 'box' (z=1)
    float range = m_farClip - m_nearClip;
    
    // x: -0.84375 y: 0.4444444444444444 z: -0.8998998998998999
    Vector deviceCoord( screen_x / (float)width, ( height - screen_y ) / (float)height, depth / range );
    deviceCoord *= 2.0f;
    deviceCoord -= Vector::one;
    
    if ( m_projectionMode == ProjectionMode::Perspective )
    {
        // calculate half width and height at the near clip plane
        Vector halfSize;
        getPerspectiveHalfSize( halfSize, m_fov, m_aspectRatio, m_nearClip );
        
        // scale by normalized screen coordinates
        halfSize.x *= deviceCoord.x;
        halfSize.y *= deviceCoord.y;
        
        // swap for my engine?
        // float temp = halfSize.y;
        // halfSize.y = halfSize.z;
        // halfSize.z = temp;
        
        // transform to world space
        Matrix inverseViewMatrix = getWorldTransform();
        halfSize.z = -m_nearClip; // don't negate near for my engine?
        Vector point = inverseViewMatrix.transformPoint( halfSize );
        
        // point along camera->_point ray at distance z from the camera
        Vector cameraPosition = getWorldPosition();
        worldCoord = ( point - cameraPosition ).normalized();
        worldCoord *= depth;
        worldCoord += cameraPosition;
    }
    else
    {
        // update viewProjectionMatrix

        Matrix inverseViewProjMatrix = m_viewProjectionMatrix.inverse();
        
        // transform to world space
        worldCoord = inverseViewProjMatrix.transformPoint( deviceCoord );
    }

    return Vector( worldCoord.x, -worldCoord.z, worldCoord.y );
}

void CameraComponent::setProjectionMode( ProjectionMode mode )
{
    m_projectionMode = mode;
}

CameraComponent::ProjectionMode CameraComponent::getProjectionMode() const
{
    return m_projectionMode;
}

const Matrix& CameraComponent::getProjectionMatrix()
{
    if (  m_projectionMatrixDirty == true )
    {
        m_projectionMatrixDirty = false;
        updateProjectionMatrix();
    }

    return m_projectionMatrix;
}

void CameraComponent::updateProjectionMatrix()
{
    if ( m_projectionMode == CameraComponent::ProjectionMode::Perspective )
    {
        m_projectionMatrix = PerspectiveMatrix( m_fov, m_aspectRatio, m_nearClip, m_farClip );
    }
    else
    {
        float height = m_orthographicSize;
        float width = m_aspectRatio * height;
        
        m_projectionMatrix = OrthoMatrix( -width, width, -height, height, m_nearClip, m_farClip );
    }
}

const Matrix& CameraComponent::getViewMatrix() const
{
    return m_viewMatrix;
}

const Matrix& CameraComponent::getViewProjectionMatrix() const
{
    return m_viewProjectionMatrix;
}

const Matrix& CameraComponent::getNormalMatrix() const
{
    return m_normalMatrix;
}

const CameraComponent::CameraViewport& CameraComponent::getViewport() const
{
    return m_viewport;
}

void CameraComponent::setFov( float fov )
{
    m_fov = fov * Deg2Rad;
    m_projectionMatrixDirty = true;
}

float CameraComponent::getFov() const
{
    return m_fov;
}

void CameraComponent::setAspectRatio( float aspect )
{
    m_aspectRatio = aspect;
    m_projectionMatrixDirty = true;
}

float CameraComponent::getAspectRatio() const
{
    return m_aspectRatio;
}

void CameraComponent::setNearClip( float nearClip )
{
    m_nearClip = nearClip;
    m_projectionMatrixDirty = true;
}

float CameraComponent::getNearClip() const
{
    return m_nearClip;
}

void CameraComponent::setFarClip( float farClip )
{
    m_farClip = farClip;
    m_projectionMatrixDirty = true;
}

float CameraComponent::getFarClip() const
{
    return m_farClip;
}

void CameraComponent::setBackgroundColor( const Color& color )
{
    m_backgroundColor = color;
}

const Color& CameraComponent::getBackgroundColor() const
{
    return m_backgroundColor;
}

