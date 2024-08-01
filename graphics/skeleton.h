// Oleg Kotov

#pragma once

#include "okobase.h"

struct SkeletalJoint
{
    std::string m_name;
    
    SkeletalJoint* m_parent = nullptr;
    std::vector<SkeletalJoint*> m_children;
    
    Vector m_position;
    Quaternion m_rotation;

    Matrix m_localMatrix;  // matrix from position and rotation
    Matrix m_worldMatrix;  // same but world space

    Matrix m_localMatrixTemp;
    Matrix m_worldMatrixTemp;

    Matrix m_inverseBindMatrix; // m_invBindPose
    Matrix m_offsetMatrix; // don't use

    // Matrix m_worldMatrix;  // joint-to-world space matrix: worldMatrix = parent->worldMatrix * localMatrix
    Matrix m_jointMatrix;  // JointMatrix = parent->worldMatrix.inverse() * worldMatrix * inverseBindMatrix // this send to shader?

	void calcInverseBindTransform( const Matrix& parentBindTransform )
	{
        Matrix bindTransform = parentBindTransform * m_localMatrix;
        Matrix inverseBindTransform = bindTransform.inverse();

        m_worldMatrix = bindTransform;
        m_offsetMatrix = inverseBindTransform;

        for ( auto& child : m_children )
        {
            child->calcInverseBindTransform( bindTransform );
        }
	}
};

struct Skeleton
{
    SkeletalJoint* m_root = nullptr;
    std::vector<SkeletalJoint*> m_joints; // Skeletal pose.
    std::vector<Matrix> m_jointMatrices;  // The final matrix to send to the shader.
};

