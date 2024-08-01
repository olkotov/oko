// Oleg Kotov

#pragma once

#include "primitivecomponent.h"
#include <stdint.h>

class StaticMesh;
class Skeleton;
struct Animation;
struct JointAnimChannel;
class Material;

class ENGINE_API SkeletalMeshComponent : public PrimitiveComponent
{    
public:
    
	SkeletalMeshComponent();
    ~SkeletalMeshComponent();
    
	void setMesh( StaticMesh* mesh );
	StaticMesh* getMesh() const;

	void setMaterial( Material* material, int32_t surfaceIndex = 0 );
    void setMaterial( const std::string& filename, int32_t surfaceIndex = 0 );
	Material* getMaterial( int32_t surfaceIndex = 0 ) const;

	void setSkeleton( Skeleton* skeleton );
	Skeleton* getSkeleton() const;

    void tick( float deltaTime ) override;

    void playAnimation( Animation* animation, bool loop = false );
    void updateSkeletonJoints();

private:

	float calcScaleFactor( float prevKeyframeTime, float nextKeyframeTime ) const;

	Vector calcInterpolatedPosition( const JointAnimChannel* animChannel );
	Quaternion calcInterpolatedRotation( const JointAnimChannel* animChannel );
	Vector calcInterpolatedScale( const JointAnimChannel* animChannel );

private:

	friend class RenderingSystem;

	StaticMesh* m_mesh = nullptr;
	Skeleton* m_skeleton = nullptr;
	Animation* m_currentAnimation = nullptr;

	float m_currentFrame = 0.0f;
	std::vector<Material*> m_materialOverrides;

	void createCollisionShape() override {}
};

