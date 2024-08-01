// Oleg Kotov

#pragma once

#include "entitycomponent.h"

class ENGINE_API SceneComponent : public EntityComponent
{    
public:

    /// Recurses up the list of parents and returns true if this scene component is a descendant of the possibleParent.
    bool isChildOf( SceneComponent* possibleParent );

    void attachTo( SceneComponent* newParent );
    SceneComponent* getParent() const;
    
    // void setRelativeTransform();
    const Matrix& getLocalMatrix();
    
    // void UpdateComponentToWorld();
    void setWorldTransform( const Matrix& matrix ); // check it!
    const Matrix& getWorldTransform();

    const Matrix& getWorldToLocalMatrix();
    const Matrix& getWorldMatrixInverseTranspose();
    
    void setWorldPositionAndRotation( const Vector& position, const Quaternion& rotation, bool teleportPhysics = true );
    void setWorldPosition( const Vector& position, bool teleportPhysics = true );
    void setWorldEulerAngles( const Vector& eulerAngles, bool teleportPhysics = true );
    void setWorldRotation( const Quaternion& rotation, bool teleportPhysics = true );
    
    void translate( const Vector& translation, bool teleportPhysics = true );
    void rotate( const Vector& rotation, bool teleportPhysics = true );
    void rotate( const Quaternion& rotation, bool teleportPhysics = true );
    
    Vector getWorldPosition();
    Vector getWorldEulerAngles();
    Quaternion getWorldRotation();
    
    void setRelativePositionAndRotation( const Vector& position, const Quaternion& rotation, bool teleportPhysics = true );
    void setRelativePosition( const Vector& position, bool teleportPhysics = true );
    void setRelativeEulerAngles( const Vector& eulerAngles, bool teleportPhysics = true );
    void setRelativeRotation( const Quaternion& rotation, bool teleportPhysics = true );
    void setRelativeScale( const Vector& scale );
    
    void translateLocal( const Vector& translation, bool teleportPhysics = true );
    void rotateLocal( const Vector& rotation, bool teleportPhysics = true );
    void rotateLocal( const Quaternion& rotation, bool teleportPhysics = true );
    
    Vector getRelativePosition() const;
    Vector getRelativeEulerAngles() const;
    Quaternion getRelativeRotation() const;
    Vector GetRelativeScale() const;
    
    Vector getForwardDirection();
    Vector getRightDirection();
    Vector getUpDirection();

    void recalculateMatrices();

private:

    void markLocalTransformDirty();
    void markWorldTransformDirty();

private:

	SceneComponent* m_parent = nullptr;
	std::vector<SceneComponent*> m_children;

	Vector m_localPosition = Vector::zero;
	Quaternion m_localRotation = Quaternion::Identity;
	Vector m_localScale = Vector::one;

    Matrix m_localMatrix = Matrix::Identity;
    bool m_dirtyLocal = true; // false

	Matrix m_localToWorldMatrix = Matrix::Identity;
	bool m_dirtyWorld = true; // false

    Matrix m_worldToLocalMatrix = Matrix::Identity;
    Matrix m_worldMatrixInverseTranspose = Matrix::Identity;
};

