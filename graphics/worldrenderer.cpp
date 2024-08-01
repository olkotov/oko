// Oleg Kotov

#include "worldrenderer.h"
#include "okoengine.h"
#include "resourcemanager.h"
#include "staticmeshcomponent.h"
#include "skeletalmeshcomponent.h"
#include "staticmesh.h"
#include "skeletalmesh.h"
#include "surface.h"
#include "cameracomponent.h"
#include "material.h"
#include "graphics.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include "log.h"
#include "orthomatrix.h"
#include "lookatmatrix.h"
#include "perspectivematrix.h"
#include "skeleton.h"
#include "entity.h"

#include <GL/glew.h>

#define MAX_JOINTS 100

WorldRenderer::WorldRenderer( Graphics* graphics )
{
	m_graphics = graphics;

	initDefaultMaterial();

	// init frame buffer

	//glGenFramebuffers( 1, &m_framebuffer );
	//glBindFramebuffer( GL_FRAMEBUFFER, m_framebuffer );

	//uint16_t screenWidth, screenHeight;
	//m_graphics->window()->getResolution( screenWidth, screenHeight );

	//// color texture

	//glGenTextures( 1, &m_colorTexture );
	//glBindTexture( GL_TEXTURE_2D, m_colorTexture );
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//glBindTexture( GL_TEXTURE_2D, 0 );

	//glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0 );

	//// depth buffer

	//glGenRenderbuffers( 1, &m_depthBuffer );
	//glBindRenderbuffer( GL_RENDERBUFFER, m_depthBuffer );
	//glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight );
	//glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	//glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer );

	//// check

	//if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
	//{
	//	Warning( "framebuffer is not complete!" );
	//}

	//glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	// shaders

	// m_quadShader = engine()->resources()->getShader( "shaders/quad_fullscreen.shader" );
	// m_fxaaShader = engine()->resources()->getShader( "shaders/fxaa.shader" );
	// m_fxaaShader = engine()->resources()->getShader( "shaders/chromatic_aberration.shader" );

	// quad geometry

	// createQuadVertexArray();

	// initShadowFramebuffer();

	// m_shadowsShader = engine()->resources()->getShader( "shaders/shadows" );

	// m_diffuseIBL = engine()->resources()->getTexture( "textures/environment/Arches_E_PineTree_Env.hdr" );
	// m_specularIBL = engine()->resources()->getTexture( "textures/environment/Arches_E_PineTree_3k.hdr" );
}

void WorldRenderer::initDefaultMaterial()
{
	Shader* shader = new Shader();
	// shader->setData();

	// m_defaultMaterial = new Material( "default" );

	// m_defaultMaterial;

	// TODO: create material in code
	m_defaultMaterial = engine()->resources()->getMaterial( "materials/default.mat" );

	if ( !m_defaultMaterial )
	{
		Warning( "default material initialization failed" );
		exit( -1 );
	}
}

void WorldRenderer::initShadowFramebuffer()
{
	glGenFramebuffers( 1, &m_shadowsFramebuffer );
	glGenTextures( 1, &m_shadowsDepthMap );

    glBindTexture( GL_TEXTURE_2D, m_shadowsDepthMap );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowMapSize, m_shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );

    glBindFramebuffer( GL_FRAMEBUFFER, m_shadowsFramebuffer );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowsDepthMap, 0 );
    glDrawBuffer( GL_NONE );
    glReadBuffer( GL_NONE );

    if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
    {
        Warning( "shadow framebuffer is not complete" );
    }

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void WorldRenderer::addStaticMeshComponent( StaticMeshComponent* meshComponent )
{
	// addUnique

	for ( int32_t i = 0; i < m_staticMeshes.size(); ++i )
	{
		if ( m_staticMeshes[i] == meshComponent ) return;
	}

	m_staticMeshes.push_back( meshComponent );
}

void WorldRenderer::removeStaticMeshComponent( StaticMeshComponent* meshComponent )
{
	// removeSwap

	auto it = std::find( m_staticMeshes.begin(), m_staticMeshes.end(), meshComponent );

	if ( it != m_staticMeshes.end() )
	{
		std::iter_swap( it, m_staticMeshes.end() - 1 );
		m_staticMeshes.pop_back();
	}
}

void WorldRenderer::addSkeletalMesh( SkeletalMeshComponent* meshComponent )
{
	// addUnique

	for ( int32_t i = 0; i < m_skeletalMeshes.size(); ++i )
	{
		if ( m_skeletalMeshes[i] == meshComponent ) return;
	}

	m_skeletalMeshes.push_back( meshComponent );
}

void WorldRenderer::removeSkeletalMesh( SkeletalMeshComponent* meshComponent )
{
	// removeSwap

	auto it = std::find( m_skeletalMeshes.begin(), m_skeletalMeshes.end(), meshComponent );

	if ( it != m_skeletalMeshes.end() )
	{
		std::iter_swap( it, m_skeletalMeshes.end() - 1 );
		m_skeletalMeshes.pop_back();
	}
}

void WorldRenderer::addLight( LightSourceComponent* lightComponent )
{
	// addUnique

	for ( int32_t i = 0; i < m_lights.size(); ++i )
	{
		if ( m_lights[i] == lightComponent ) return;
	}

	m_lights.push_back( lightComponent );
}

void WorldRenderer::removeLight( LightSourceComponent* lightComponent )
{
	// removeSwap

	auto it = std::find( m_lights.begin(), m_lights.end(), lightComponent );

	if ( it != m_lights.end() )
	{
		std::iter_swap( it, m_lights.end() - 1 );
		m_lights.pop_back();
	}
}
 
void WorldRenderer::setIrradianceCubeMap( Texture* texture )
{
	m_diffuseIBL = texture;
}

void WorldRenderer::setPrefilterCubeMap( Texture* texture )
{
	m_specularIBL = texture;
}

void WorldRenderer::setBrdfLUTMap( Texture* texture )
{
	m_brdfLUT = texture;
}

void WorldRenderer::drawWorld()
{
	m_camera = m_graphics->getActiveCamera();
    assert( m_camera );
    
	m_viewMatrix = m_camera->getViewMatrix();
	m_projectionMatrix = m_camera->getProjectionMatrix();
	m_viewProjectionMatrix = m_camera->getViewProjectionMatrix();
	m_viewProjectionMatrixInverse = m_viewProjectionMatrix.inverse();

	// TODO: calc shadow box
	Matrix lightProjection = OrthoMatrix( -22.0f, 22.0f, -22.0f, 22.0f, 1.0f, 30.0f );

	Vector lightPosition = Vector( 0.3f, 0.4f, 0.45f );
	lightPosition *= 20.0f;

	Vector lightDirection = Vector( 0.0f, 0.0f, 0.0f );

	Vector lightTargetPoint = lightDirection; // lightPosition + lightDirection;

	Matrix lightView = LookAtMatrix( lightPosition, lightTargetPoint );
	m_lightSpaceMatrix = lightProjection * lightView;

	std::vector<RenderItem> renderItems;
	processMeshComponents( renderItems );

	// frustumCulling( renderItems );

	// drawShadowsToMap( renderItems );

	std::vector<RenderItem> opaqueRenderItems;
	std::vector<RenderItem> transparentRenderItems;

	for ( const auto& renderItem : renderItems )
	{
		if ( !renderItem.material->isTransparent() )
		{
			opaqueRenderItems.push_back( renderItem );
		}
		else
		{
			transparentRenderItems.push_back( renderItem );
		}
	}

	sortTransparentRenderItems( transparentRenderItems );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	const Color& clearColor = m_camera->getBackgroundColor();
	int viewportWidth = m_graphics->getWindowWidth();
	int viewportHeight = m_graphics->getWindowHeight();

	glViewport( 0, 0, viewportWidth, viewportHeight );

    glClearColor( clearColor.r, clearColor.g, clearColor.b, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for ( const auto& renderItem : opaqueRenderItems )
	{
		drawRenderItem( renderItem );
	}

	for ( const auto& renderItem : transparentRenderItems )
	{
		drawRenderItem( renderItem );
	}

	// ugly animated version

	glBindVertexArray( 0 );
}

void WorldRenderer::drawShadowsToMap( const std::vector<RenderItem>& renderItems ) const
{
	m_shadowsShader->bind();

	glViewport( 0, 0, m_shadowMapSize, m_shadowMapSize );
	glBindFramebuffer( GL_FRAMEBUFFER, m_shadowsFramebuffer );
	glClear( GL_DEPTH_BUFFER_BIT );

	glCullFace( GL_FRONT );

	for ( const auto& renderItem : renderItems )
	{
		Matrix mvpMatrix = m_lightSpaceMatrix * renderItem.modelMatrix;

		m_shadowsShader->setMatrixUniform( "_MVPMatrix", mvpMatrix.transposed() );

		glBindVertexArray( renderItem.surface->getVertexArray() );
		glDrawElements( GL_TRIANGLES, renderItem.surface->getNumIndices(), GL_UNSIGNED_INT, 0 );
	}

	glCullFace( GL_BACK );

	m_shadowsShader->unbind();
}

void WorldRenderer::processMeshComponents( std::vector<RenderItem>& renderItems ) const
{
	for ( const auto& meshComponent : m_staticMeshes )
    {
		if ( meshComponent->getOwner()->isPendingKill() ) continue;

		assert( meshComponent );

		if ( !meshComponent->isEnabled() ) continue;
		if ( !meshComponent->getMesh() ) continue;
		if ( !meshComponent->getMesh()->hasSurfaces() ) continue;

		StaticMesh* mesh = meshComponent->getMesh();

		for ( int32_t surfaceIndex = 0; surfaceIndex < mesh->getNumSurfaces(); ++surfaceIndex )
		{
			Material* material = meshComponent->getMaterial( surfaceIndex );

			RenderItem item;

			item.surface = mesh->getSurface( surfaceIndex );
			item.material = material;

			item.isStaticMesh = true;
			item.meshComponent = meshComponent;

			item.modelMatrix = meshComponent->getWorldTransform();
			item.worldToLocalMatrix = meshComponent->getWorldToLocalMatrix();
			item.modelMatrixInverseTranspose = meshComponent->getWorldMatrixInverseTranspose();
			item.mvpMatrix = m_viewProjectionMatrix * item.modelMatrix;

			renderItems.push_back( item );
		}
	}

	for ( const auto& meshComponent : m_skeletalMeshes )
    {
		assert( meshComponent );

		if ( !meshComponent->isEnabled() ) continue;
		if ( !meshComponent->getMesh() ) continue;
		if ( !meshComponent->getMesh()->hasSurfaces() ) continue;

		StaticMesh* mesh = meshComponent->getMesh();

		for ( int32_t surfaceIndex = 0; surfaceIndex < mesh->getNumSurfaces(); ++surfaceIndex )
		{
			Material* material = meshComponent->getMaterial( surfaceIndex );

			RenderItem item;

			item.surface = mesh->getSurface( surfaceIndex );
			item.material = material;

			item.isStaticMesh = false;
			item.meshComponentAnim = meshComponent;

			item.modelMatrix = meshComponent->getWorldTransform();
			item.worldToLocalMatrix = meshComponent->getWorldToLocalMatrix();
			item.modelMatrixInverseTranspose = meshComponent->getWorldMatrixInverseTranspose();
			item.mvpMatrix = m_viewProjectionMatrix * item.modelMatrix;

			renderItems.push_back( item );
		}
	}
}

void WorldRenderer::frustumCulling( std::vector<RenderItem>& renderItems ) const
{
	// TODO: implement
}

void WorldRenderer::sortTransparentRenderItems( std::vector<RenderItem>& renderItems ) const
{
	// TODO: implement
}

void WorldRenderer::drawRenderItem( const RenderItem& renderItem ) const
{
	Surface* surface = renderItem.surface;
	Material* material = renderItem.material;

	bool isDoubleSided = material->isDoubleSided();
	bool depthEnabled = material->isDepthEnabled();

	SurfaceType surfaceType = material->getSurfaceType();
	RenderFace renderFace = material->getRenderFace();

	( isDoubleSided ) ? glDisable( GL_CULL_FACE ) : glEnable( GL_CULL_FACE );
	( depthEnabled ) ? glEnable( GL_DEPTH_TEST ) : glDisable( GL_DEPTH_TEST );

	// surface type
	// ...

	// render face
	// ...

	Shader* shader = material->getShader();
	shader->bind();

	// set vertex attributes
	surface->bindToShader( shader );

	// world renderer uniforms
	setupBaseUniforms( shader, renderItem );
	
	// material uniforms
	setupMaterialUniforms( shader, material );

	// skeletal animation joints uniform
	setupJointUniform( shader, renderItem );

	glBindVertexArray( surface->getVertexArray() );
	glDrawElements( GL_TRIANGLES, surface->getNumIndices(), GL_UNSIGNED_INT, 0 );

	shader->unbind();
}

void WorldRenderer::setupBaseUniforms( Shader* shader, const RenderItem& renderItem ) const
{
	shader->setMatrixUniform( "_ObjectToWorldMatrix", renderItem.modelMatrix.transposed() );
	shader->setMatrixUniform( "_WorldToObjectMatrix", renderItem.worldToLocalMatrix.transposed() );
	shader->setMatrixUniform( "_ModelMatrixInverseTranspose", renderItem.modelMatrixInverseTranspose.transposed() );
	shader->setMatrixUniform( "_MVPMatrix", renderItem.mvpMatrix.transposed() );

	shader->setMatrixUniform( "_ViewMatrix", m_viewMatrix.transposed() );
	shader->setMatrixUniform( "_ProjectionMatrix", m_projectionMatrix.transposed() );
	shader->setMatrixUniform( "_ViewProjectionMatrix", m_viewProjectionMatrix.transposed() );
	shader->setMatrixUniform( "_ViewProjectionMatrixInverse", m_viewProjectionMatrixInverse.transposed() );
			
	shader->setVectorUniform( "_WorldSpaceCameraPos", m_camera->getWorldPosition() );
	shader->setVectorUniform( "_WorldSpaceCameraDir", m_camera->getForwardDirection() );

	shader->setFloatUniform ( "_Time", engine()->gameClock()->getTimeSinceStartup() );

	shader->setMatrixUniform( "_LightSpaceMatrix", m_lightSpaceMatrix.transposed() );

	//int8_t textureSlot = 0;
	//glActiveTexture( GL_TEXTURE0 + textureSlot );
	//glBindTexture( GL_TEXTURE_2D, m_shadowsDepthMap );
	//shader->setIntegerUniform( "_ShadowMap", textureSlot );

	//textureSlot = 1;
	//glActiveTexture( GL_TEXTURE0 + textureSlot );
	//glBindTexture( GL_TEXTURE_CUBE_MAP, m_diffuseIBL->getTextureId() );
	//shader->setIntegerUniform( "_DiffuseIBL", textureSlot );

	//textureSlot = 2;
	//glActiveTexture( GL_TEXTURE0 + textureSlot );
	//glBindTexture( GL_TEXTURE_CUBE_MAP, m_specularIBL->getTextureId() );
	//shader->setIntegerUniform( "_SpecularIBL", textureSlot );

	//textureSlot = 3;
	//glActiveTexture( GL_TEXTURE0 + textureSlot );
	//glBindTexture( GL_TEXTURE_2D, m_brdfLUT->getTextureId() );
	//shader->setIntegerUniform( "_BRDFIntegrationMap", textureSlot );
}

void WorldRenderer::setupMaterialUniforms( Shader* shader, Material* material ) const
{
	for ( const auto& parameter : material->getFloatParameters() )
	{
		shader->setFloatUniform( parameter.name, parameter.value );
	}

	for ( const auto& parameter : material->getVectorParameters() )
	{
		shader->setVectorUniform( parameter.name, parameter.value );
	}

	for ( const auto& parameter : material->getColorParameters() )
	{
		shader->setColorUniform( parameter.name, parameter.value );
	}
	
	int8_t textureSlot = 4;
	for ( const auto& parameter : material->getTextureParameters() )
	{
		if ( !parameter.value ) continue;

		glActiveTexture( GL_TEXTURE0 + textureSlot );

		if ( parameter.value->getTextureType() == Texture::Type::Texture2D )
		{
			glBindTexture( GL_TEXTURE_2D, parameter.value->getTextureId() );
		}

		else if ( parameter.value->getTextureType() == Texture::Type::CubeTexture )
		{
			glBindTexture( GL_TEXTURE_CUBE_MAP, parameter.value->getTextureId() );
		}
				
		shader->setIntegerUniform( parameter.name, textureSlot );
				
		textureSlot++;
	}
}

void WorldRenderer::setupJointUniform( Shader* shader, const RenderItem& renderItem ) const
{
	if ( renderItem.isStaticMesh ) return;

	int32_t jointMatrixLocations[MAX_JOINTS];

	for ( int32_t i = 0; i < MAX_JOINTS; ++i )
	{
		char varName[128];
		memset( varName, 0, sizeof( varName ) );
		_snprintf_s( varName, sizeof( varName ), "_JointMatrices[%d]", i );
		jointMatrixLocations[i] = glGetUniformLocation( shader->getNativeHandle(), varName );
	}

	Skeleton* skeleton = renderItem.meshComponentAnim->getSkeleton();

	if ( !skeleton ) return;

	std::vector<Matrix> jointMatrices = skeleton->m_jointMatrices;

	//for ( int32_t i = 0; i < jointMatrices.size(); ++i )
	//{
	//	if ( i >= MAX_JOINTS )
	//	{
	//		assert( 0 );
	//	}

	//	Matrix jointMatrix = skeleton->m_joints[i]->m_jointMatrix;

	//	// Matrix jointMatrix = jointMatrices[i];
	//	jointMatrix = jointMatrix.transposed();

	//	glUniformMatrix4fv( jointMatrixLocations[i], 1, GL_FALSE, &jointMatrix[0][0] );
	//}

	for ( int32_t i = 0; i < MAX_JOINTS; ++i )
	{
		Matrix jointMatrix;

		if ( i < skeleton->m_joints.size() )
		{
			// jointMatrix = jointMatrices[i];
			jointMatrix = skeleton->m_joints[i]->m_jointMatrix;
		}

		jointMatrix = jointMatrix.transposed();

		glUniformMatrix4fv( jointMatrixLocations[i], 1, GL_FALSE, &jointMatrix[0][0] );
	}
}

