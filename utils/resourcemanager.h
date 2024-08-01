// Oleg Kotov

#pragma once

#include "okobase.h"

class SkeletalMesh;
class StaticMesh;
struct Skeleton;
struct Animation;
class Shader;
class Texture;
class Material;
class Sound;
// class CubeTexture;
class Asset;

#include <iostream>
#include <fstream>

class ENGINE_API ResourceManager
{    
public:
    
    ResourceManager();
    ~ResourceManager();

    void setContentDirectory( const std::string& dir );
    const std::string& getContentDirectory() const;

    SkeletalMesh* getSkeletalMesh( const std::string& filepath );
    Skeleton* getSkeleton( const std::string& filepath );
    Skeleton* getSkeletonJson( const std::string& filepath );
    Animation* getAnimSequence( const std::string& filepath );
    Animation* getAnimSequenceJson( const std::string& filepath );

    StaticMesh* getStaticMesh2( const std::string& file );
    StaticMesh* getStaticMesh( const std::string& file );
    StaticMesh* getMesh( const std::string& file );

    Shader* getShader( const std::string& file );

    Texture* getTexture( const std::string& file );
    
	Material* createMaterial(const std::string& name, Shader* shader = nullptr );
    Material* getMaterial( const std::string& name );
    
    Sound* getSound( const std::string& file );

private:

    std::string m_assetsRootDirectory;

    std::map<std::string, Asset*> m_assets;

	// Resource caches
	std::map<std::string, StaticMesh*>   m_meshes;

    std::map<std::string, SkeletalMesh*> m_skeletalMeshes;
	std::map<std::string, Skeleton*>     m_skeletons;
    std::map<std::string, Animation*> m_animations;

    std::map<std::string, Material*>     m_materials;
    std::map<std::string, Shader*>       m_shaders;
	std::map<std::string, Texture*>      m_textures;

	std::map<std::string, Sound*>        m_sounds;
    // std::map<std::string, CubeTexture*>  m_cubeTextures;
    
};

