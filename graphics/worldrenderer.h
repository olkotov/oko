// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "matrix.h"

#include <stdint.h>
#include <vector>

class StaticMeshComponent;
class SkeletalMeshComponent;
class LightSourceComponent;
class CameraComponent;
class Material;
class Shader;
class Graphics;
class Surface;
class Texture;

class ENGINE_API WorldRenderer
{        
public:

	struct RenderItem
	{
		Surface* surface;
		Material* material;

		bool isStaticMesh;
		StaticMeshComponent* meshComponent;
		SkeletalMeshComponent* meshComponentAnim;

		Matrix modelMatrix;
		Matrix worldToLocalMatrix;
		Matrix modelMatrixInverseTranspose;
		Matrix mvpMatrix;
	};

public:
    
	WorldRenderer( Graphics* graphics );

private:

	void initDefaultMaterial();
	void initShadowFramebuffer();

public:

	Material* getDefaultMaterial() const { return m_defaultMaterial; }

	void addStaticMeshComponent( StaticMeshComponent* meshComponent );
	void removeStaticMeshComponent( StaticMeshComponent* meshComponent );
	
	void addSkeletalMesh( SkeletalMeshComponent* meshComponent );
	void removeSkeletalMesh( SkeletalMeshComponent* meshComponent );

	void addLight( LightSourceComponent* lightComponent );
	void removeLight( LightSourceComponent* lightComponent );

	void setIrradianceCubeMap( Texture* texture );
	void setPrefilterCubeMap( Texture* texture );
	void setBrdfLUTMap( Texture* texture );

	// draw

	void drawWorld();

private:

	void drawShadowsToMap( const std::vector<RenderItem>& renderItems ) const;

	void processMeshComponents( std::vector<RenderItem>& renderItems ) const;
	void frustumCulling( std::vector<RenderItem>& renderItems ) const;
	void sortTransparentRenderItems( std::vector<RenderItem>& renderItems ) const;

	void drawRenderItem( const RenderItem& renderItem ) const;
	void setupBaseUniforms( Shader* shader, const RenderItem& renderItem ) const;
	void setupMaterialUniforms( Shader* shader, Material* material ) const;
	void setupJointUniform( Shader* shader, const RenderItem& renderItem ) const;

private:

	Material* m_defaultMaterial = nullptr;

	std::vector<StaticMeshComponent*> m_staticMeshes;
	std::vector<SkeletalMeshComponent*> m_skeletalMeshes;
	std::vector<LightSourceComponent*> m_lights;

	uint32_t m_framebuffer = 0;
	uint32_t m_colorTexture = 0;
	uint32_t m_depthBuffer = 0;

	Graphics* m_graphics = nullptr;

private:

	class Shader* m_shadowsShader = nullptr;

	uint16_t m_shadowMapSize = 1024;

	uint32_t m_shadowsFramebuffer = 0;
	uint32_t m_shadowsDepthMap = 0;

private:

	CameraComponent* m_camera = nullptr;

	Matrix m_viewMatrix;
	Matrix m_projectionMatrix;
	Matrix m_viewProjectionMatrix;
	Matrix m_viewProjectionMatrixInverse;
	Matrix m_lightSpaceMatrix;

private:

	Texture* m_diffuseIBL = nullptr;
	Texture* m_specularIBL = nullptr;
	Texture* m_brdfLUT = nullptr;
};

