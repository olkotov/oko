// Oleg Kotov

#include "skeletalmeshcomponent.h"
#include "skeletalmesh.h"
#include "staticmesh.h"
#include "surface.h"
#include "skeleton.h"
#include "animsequence.h"
#include "transformmatrix.h"
#include "okoengine.h"
#include "material.h"
#include "graphics.h"
#include "worldrenderer.h"
#include "resourcemanager.h"

SkeletalMeshComponent::SkeletalMeshComponent()
{
    engine()->graphics()->worldRenderer()->addSkeletalMesh( this );
}

SkeletalMeshComponent::~SkeletalMeshComponent()
{
    engine()->graphics()->worldRenderer()->removeSkeletalMesh( this );
}

void SkeletalMeshComponent::setMesh( StaticMesh* skeletalMesh )
{
    m_mesh = skeletalMesh;

    m_materialOverrides.clear();
	m_materialOverrides.resize( m_mesh->getNumSurfaces() );
}

StaticMesh* SkeletalMeshComponent::getMesh() const
{
    return m_mesh;
}

void SkeletalMeshComponent::setMaterial( Material* material, int32_t surfaceIndex /* = 0 */ )
{
	m_materialOverrides[surfaceIndex] = material;
}

void SkeletalMeshComponent::setMaterial( const std::string& filename, int32_t surfaceIndex /* = 0 */ )
{
	Material* material = engine()->resources()->getMaterial( filename );
	assert( material );

	setMaterial( material, surfaceIndex );
}

Material* SkeletalMeshComponent::getMaterial( int32_t surfaceIndex /* = 0 */ ) const
{
	// try material overrides
	if ( surfaceIndex < m_materialOverrides.size() && m_materialOverrides[surfaceIndex] )
	{
		return m_materialOverrides[surfaceIndex];
	}

	// try mesh material
	if ( m_mesh && surfaceIndex < m_mesh->getNumSurfaces() && m_mesh->getSurface( surfaceIndex )->getMaterial() )
	{
		return m_mesh->getSurface( surfaceIndex )->getMaterial();
	}

	// or just return default material
	return engine()->graphics()->worldRenderer()->getDefaultMaterial();
}

void SkeletalMeshComponent::setSkeleton( Skeleton* skeleton )
{
    m_skeleton = skeleton;
}

Skeleton* SkeletalMeshComponent::getSkeleton() const
{
    return m_skeleton;
}

void SkeletalMeshComponent::playAnimation( Animation* animation, bool loop /* = false */ )
{
    m_currentAnimation = animation;
    m_currentFrame = 0.0f;
}

/*

// частота кадров текущей анимации, выраженная в кадрах в секунду
float frameRate = (float)m_animation.m_frameRate;

// общее количество кадров, прошедших с предыдущего кадра в игре
float framesElapsed = frameRate * deltaTime;

// продвигаем анимацию
m_currentFrame += framesElapsed;

// Функция fmodf используется для вычисления остатка от деления
// текущего кадра на общее количество кадров в анимации.
// Это позволяет обеспечить зацикленное проигрывание анимации.
m_currentFrame = fmodf( m_currentFrame, (float)m_animation.m_numFrames );

m_currentFrame = fmodf( m_currentFrame + deltaTime * m_currentAnimation->m_frameRate, m_currentAnimation->m_numFrames );

*/

void SkeletalMeshComponent::tick( float deltaTime )
{
    if ( !m_mesh || !m_currentAnimation || !m_skeleton ) return;
    
    float framesElapsed = m_currentAnimation->m_framerate * deltaTime;
    m_currentFrame += framesElapsed;

    // loop
    m_currentFrame = fmodf( m_currentFrame, m_currentAnimation->m_numFrames - 1 );
    
    // m_currentFrame = 5.0f;

    updateSkeletonJoints(); // update skeleton pose matrices
}

void printMatrix( const char* matrixName, const Matrix& matrix )
{
	printf( "> %s\n", matrixName );

	printf( "    %f %f %f %f \n", matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3] );
	printf( "    %f %f %f %f \n", matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3] );
	printf( "    %f %f %f %f \n", matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3] );
	printf( "    %f %f %f %f \n", matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3] );
}

void SkeletalMeshComponent::updateSkeletonJoints()
{
    int i = 0;

    for ( auto& joint : m_skeleton->m_joints )
    {
        joint->m_localMatrixTemp = joint->m_worldMatrix; //  = TransformMatrix( joint->m_position, joint->m_rotation ); // default

        Matrix localMatrix = joint->m_localMatrixTemp;

        const JointAnimChannel* animChannel = m_currentAnimation->getChannelByJointName( joint->m_name );

        // we have animation data for this joint
        if ( animChannel )
        {
            Vector position = calcInterpolatedPosition( animChannel );
            Quaternion rotation = calcInterpolatedRotation( animChannel );
            Vector scale = calcInterpolatedScale( animChannel );

            localMatrix = TransformMatrix( position, rotation, scale );

            // 0.0 0.598744 -0.038799
            Vector jointPosWS = joint->m_worldMatrix.getTranslation(); // joint->m_position + joint->m_worldMatrix.transformPoint( position );
            Vector transformedVector = joint->m_worldMatrix.transformVector( position );
            Vector result = jointPosWS + transformedVector;

            if ( joint->m_name == "Chest" )
            {
                // printf("");
            }
        }

        if ( !joint->m_parent )
        {
            joint->m_worldMatrixTemp = localMatrix;
        }
        else
        {
            joint->m_worldMatrixTemp = joint->m_parent->m_worldMatrixTemp * localMatrix;
        }

        // joint matrix will be send to shader
        joint->m_jointMatrix = joint->m_worldMatrixTemp * joint->m_inverseBindMatrix;
        // joint->m_jointMatrix = joint->m_inverseBindMatrix * joint->m_worldMatrixTemp;

        /*if ( joint->m_name == "Foot.R" )
        {
            printMatrix( "jointMatrix", joint->m_jointMatrix );
        }*/

        std::string matrixName = "jointMatrix - " + std::to_string( i );
        // printMatrix( matrixName.c_str(), joint->m_jointMatrix );

        i++;
    }

    printf("");
}

//void SkeletalMeshComponent::updateSkeletonJoints()
//{    
//    int32_t numJoints = m_skeleton->m_joints.size();
//
//    for ( int32_t i = 0; i < numJoints; ++i )
//    {
//        SkeletalJoint* joint = m_skeleton->m_joints[i];
//
//        // printf( "%s\n", joint->m_name.c_str() );
//
//        joint->m_localMatrix = TransformMatrix( joint->m_position, joint->m_rotation ); // pre-calculate this?
//
//        Matrix localMatrix = joint->m_localMatrix;
//
//        const JointAnimChannel* animChannel = m_currentAnimation->getChannelByJointName( joint->m_name );
//
//        // we have animation data for this joint
//        if ( animChannel )
//        {
//            Vector position = calcInterpolatedPosition( animChannel );
//            Quaternion rotation = calcInterpolatedRotation( animChannel );
//            Vector scale = calcInterpolatedScale( animChannel );
//
//            localMatrix = TransformMatrix( position, rotation, scale );
//
//            // printMatrix( "localMatrix", localMatrix );
//        }
//
//        if ( !joint->m_parent )
//        {
//            joint->m_worldMatrix = localMatrix;
//        }
//        else
//        {
//            joint->m_worldMatrix = joint->m_parent->m_worldMatrix * localMatrix;
//        }
//
//        // joint->m_inverseBindMatrix = joint->m_worldMatrix.inverse();
//
//        // joint matrices is offsetMatrix array?
//        m_skeleton->m_jointMatrices[i] = joint->m_worldMatrix * joint->m_inverseBindMatrix;
//
//        joint->m_jointMatrix = joint->m_worldMatrix * joint->m_inverseBindMatrix;
//
//        // m_skeleton->m_jointMatrices[i] = joint->m_inverseBindMatrix * joint->m_worldMatrix;
//
//        /*if ( joint->m_name == "Armature_Foot_R" && animChannel != nullptr )
//        {
//            printMatrix( "localMatrix", joint->m_localMatrix );
//            printMatrix( "localMatrix (anim)", localMatrix );
//            printMatrix( "worldMatrix", joint->m_worldMatrix );
//            printMatrix( "inverseBindMatrix", joint->m_inverseBindMatrix );
//            printMatrix( "offsetMatrix", joint->m_offsetMatrix );
//            printMatrix( "jointMatrix", m_skeleton->m_jointMatrices[i] );
//        }*/
//
//        // printMatrix( "jointMatrix", m_skeleton->m_jointMatrices[i] );
//    }
//}

float SkeletalMeshComponent::calcScaleFactor( float prevKeyframeTime, float nextKeyframeTime ) const
{
    float midwayLength = m_currentFrame - prevKeyframeTime; // currentTime == animationTime
    float framesDifference = nextKeyframeTime - prevKeyframeTime;

    return midwayLength / framesDifference;
}

Vector SkeletalMeshComponent::calcInterpolatedPosition( const JointAnimChannel* animChannel )
{
    if ( animChannel->m_positionKeyframes.size() == 1 )
    {
        return animChannel->m_positionKeyframes[0].m_value;
    }
    
    int32_t prevKeyframeIndex = animChannel->findPositionKeyframe( m_currentFrame );
    int32_t nextKeyframeIndex = prevKeyframeIndex + 1;
    
    // check out of (not shure)
    if ( nextKeyframeIndex >= animChannel->m_positionKeyframes.size() )
    {
        nextKeyframeIndex = 0;
    }
    
    float prevKeyframeTime = animChannel->m_positionKeyframes[prevKeyframeIndex].m_time;
    float nextKeyframeTime = animChannel->m_positionKeyframes[nextKeyframeIndex].m_time;
    
    float scaleFactor = calcScaleFactor( prevKeyframeTime, nextKeyframeTime );
    
    Vector prevKeyframePosition = animChannel->m_positionKeyframes[prevKeyframeIndex].m_value;
    Vector nextKeyframePosition = animChannel->m_positionKeyframes[nextKeyframeIndex].m_value;
    
    Vector currentPosition = Vector::lerp( prevKeyframePosition, nextKeyframePosition, scaleFactor );

    return currentPosition;
}

Quaternion SkeletalMeshComponent::calcInterpolatedRotation( const JointAnimChannel* animChannel )
{
    if ( animChannel->m_rotationKeyframes.size() == 1 )
    {
        return animChannel->m_rotationKeyframes[0].m_value;
    }
    
    int32_t prevKeyframeIndex = animChannel->findPositionKeyframe( m_currentFrame );
    int32_t nextKeyframeIndex = prevKeyframeIndex + 1;
    
    // check out of (not shure)
    if ( nextKeyframeIndex >= animChannel->m_rotationKeyframes.size() )
    {
        nextKeyframeIndex = 0;
    }
    
    float prevKeyframeTime = animChannel->m_rotationKeyframes[prevKeyframeIndex].m_time;
    float nextKeyframeTime = animChannel->m_rotationKeyframes[nextKeyframeIndex].m_time;
    
    float scaleFactor = calcScaleFactor( prevKeyframeTime, nextKeyframeTime );
    
    Quaternion prevKeyframeRotation = animChannel->m_rotationKeyframes[prevKeyframeIndex].m_value;
    Quaternion nextKeyframeRotation = animChannel->m_rotationKeyframes[nextKeyframeIndex].m_value;
    
    Quaternion currentRotation = Quaternion::slerp( prevKeyframeRotation, nextKeyframeRotation, scaleFactor );
    currentRotation.normalize();

    return currentRotation;
}

Vector SkeletalMeshComponent::calcInterpolatedScale( const JointAnimChannel* animChannel )
{
    if ( animChannel->m_scaleKeyframes.size() == 0 )
    {
        return Vector::one;
    }

    if ( animChannel->m_scaleKeyframes.size() == 1 )
    {
        return animChannel->m_scaleKeyframes[0].m_value;
    }
    
    int32_t prevKeyframeIndex = animChannel->findScaleKeyframe( m_currentFrame );
    int32_t nextKeyframeIndex = prevKeyframeIndex + 1;
    
    // check out of (not shure)
    if ( nextKeyframeIndex >= animChannel->m_scaleKeyframes.size() )
    {
        nextKeyframeIndex = 0;
    }
    
    float prevKeyframeTime = animChannel->m_scaleKeyframes[prevKeyframeIndex].m_time;
    float nextKeyframeTime = animChannel->m_scaleKeyframes[nextKeyframeIndex].m_time;
    
    float scaleFactor = calcScaleFactor( prevKeyframeTime, nextKeyframeTime );
    
    Vector prevKeyframeScale = animChannel->m_scaleKeyframes[prevKeyframeIndex].m_value;
    Vector nextKeyframeScale = animChannel->m_scaleKeyframes[nextKeyframeIndex].m_value;
    
    Vector currentScale = Vector::lerp( prevKeyframeScale, nextKeyframeScale, scaleFactor );

    return currentScale;
}

