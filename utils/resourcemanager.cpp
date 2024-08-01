// Oleg Kotov

#include "resourcemanager.h"
#include "filesystem.h"
#include "skeletalmesh.h"
#include "staticmesh.h"
#include "surface.h"
#include "vertex.h"
#include "triangle.h"
#include "skeleton.h"
#include "animsequence.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "sound.h"
#include "log.h"

#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <SOIL2/SOIL2.h>
#include <stb_image.h>

#include <AL/al.h>

#include "transformMatrix.h"

#include <tinyxml2.h>
using namespace tinyxml2;

#include <filesystem>
namespace fs = std::filesystem;

#include <nlohmann/json.hpp>

#include "materialloader.h"

std::string tolower( std::string& str )
{
	std::transform( str.begin(), str.end(), str.begin(),
		[]( unsigned char c ) { return std::tolower( c ); }
	);

	return str;
}

std::string sanitize_path( std::string& path )
{
	std::replace( path.begin(), path.end(), '\\', '/' );
	tolower( path );

	return path;
}

ResourceManager::ResourceManager()
{
	Msg( "resource manager initialize..." );

	m_assetsRootDirectory = Filesystem.getWorkingDir() + "assets/";
	sanitize_path( m_assetsRootDirectory );

    Msg( "default content directory: '%s'", m_assetsRootDirectory.c_str() );
}

ResourceManager::~ResourceManager()
{
	Msg( "resource manager shutdown" );

	for ( auto& asset : m_assets )
	{
		SafeDelete( asset.second );
	}

	m_assets.clear();

	for ( auto& asset : m_sounds )
	{
		SafeDelete( asset.second );
	}

	m_sounds.clear();
}

void ResourceManager::setContentDirectory( const std::string& dir )
{
	m_assetsRootDirectory = dir;

	Msg( "content directory changed to '%s'", m_assetsRootDirectory.c_str() );
}

const std::string& ResourceManager::getContentDirectory() const
{
	return m_assetsRootDirectory;
}

SkeletalMesh* ResourceManager::getSkeletalMesh( const std::string& filepath )
{
	// Try cache ----------------------------------------------

    auto it = m_skeletalMeshes.find( filepath );
    
	if ( it != m_skeletalMeshes.end() )
	{
		// Msg( "skeletal mesh '%s' has been retrieved from the cache", filepath.c_str() );
		return it->second;
	}

	// Load resource ------------------------------------------

	std::string path = m_assetsRootDirectory + filepath;
	Msg( "loading '%s'...", filepath.c_str() );

	std::ifstream file( path, std::ofstream::binary );

	if ( file.is_open() == false )
	{
		Warning( "unable to open '%s'! skipping...", path.c_str() );
		return nullptr;
	}

	SkeletalMesh* mesh = new SkeletalMesh();

	int32_t numSurfaces = 0;

	read( file, &numSurfaces );

	for ( int32_t i = 0; i < numSurfaces; ++i )
	{
		Surface* surface = new Surface();

		// surface name

		uint8_t surfaceNameLength = 0;
		read( file, &surfaceNameLength );

		std::string surfaceName;
		surfaceName.resize( surfaceNameLength );
		read( file, &surfaceName[0], surfaceNameLength );

		surface->setName( surfaceName );

		// surface material

		uint8_t materialPathLength = 0;
		read( file, &materialPathLength );

		std::string materialPath;
		materialPath.resize( materialPathLength );
		read( file, &materialPath[0], materialPathLength );

		Material* material = getMaterial( materialPath );
		surface->setMaterial( material );

		// vertices

		int32_t numVertices = 0;
		read( file, &numVertices );

		for ( int32_t j = 0; j < numVertices; ++j )
		{
			Vertex vertex = {};
			read( file, &vertex );
		
			surface->addVertex( vertex );
		}

		// triangles

		int32_t numTriangles = 0;
		read( file, &numTriangles );

		for ( int32_t j = 0; j < numTriangles; ++j )
		{
			Triangle triangle = {};
			read( file, &triangle );
			
			surface->addTriangle( triangle );
		}

		mesh->addSurface( surface );
	}

	mesh->flushMesh();

	file.close();
	
	m_skeletalMeshes.insert( std::make_pair( filepath, mesh ) );
    
    return mesh;
}

Skeleton* ResourceManager::getSkeleton( const std::string& filepath )
{
	// Try cache ----------------------------------------------

    auto it = m_skeletons.find( filepath );
    
	if ( it != m_skeletons.end() )
	{
		// Msg( "skeleton '%s' has been retrieved from the cache", filepath.c_str() );
		return it->second;
	}

	// Load resource ------------------------------------------

	std::string path = m_assetsRootDirectory + filepath;
	Msg( "loading '%s'...", filepath.c_str() );

	std::ifstream file( path, std::ofstream::binary );

	if ( file.is_open() == false )
	{
		Warning( "unable to open '%s'! skipping...", path.c_str() );
		return nullptr;
	}

	Skeleton* skeleton = new Skeleton();

	int32_t numJoints = 0;
	read( file, &numJoints );

	// fix it, may not here
	skeleton->m_jointMatrices.resize( numJoints );

	std::map<int32_t, SkeletalJoint*> jointIndexToJoint;

	for ( int32_t i = 0; i < numJoints; ++i )
	{
		SkeletalJoint* joint = new SkeletalJoint();

		jointIndexToJoint[i] = joint;

		// joint name

		uint8_t jointNameLength = 0;
		read( file, &jointNameLength );

		std::string jointName;
		jointName.resize( jointNameLength );
		read( file, &jointName[0], jointNameLength );

		joint->m_name = jointName;

		// parent index

		int32_t parentIndex = -1;
		read( file, &parentIndex );

		if ( parentIndex >= 0 )
		{
			SkeletalJoint* parentJoint = jointIndexToJoint[parentIndex];
			parentJoint->m_children.push_back( joint );

			joint->m_parent = parentJoint;
		}

		// inverse bind pose matrix

		read( file, &joint->m_inverseBindMatrix );

		// position

		read( file, &joint->m_position );

		// rotation

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

		read( file, &x );
		read( file, &y );
		read( file, &z );

		float t = 1.0f - ( x * x ) - ( y * y ) - ( z * z );

		if ( t >= 0.0f )
		{
			w = -sqrtf( t );
		}

		read( file, &w );

		joint->m_rotation.x = x;
		joint->m_rotation.y = y;
		joint->m_rotation.z = z;
		joint->m_rotation.w = w;

		// ---

		skeleton->m_joints.push_back( joint );
	}

	// calculate localMatrix and offsetMatrix

	for ( auto& joint : skeleton->m_joints )
	{
		joint->m_localMatrix = TransformMatrix( joint->m_position, joint->m_rotation );

		if ( !joint->m_parent )
        {
            joint->m_worldMatrix = joint->m_localMatrix;
			// joint->m_offsetMatrix = joint->m_worldMatrix.inverse();
        }
        else
        {
            joint->m_worldMatrix = joint->m_parent->m_worldMatrix * joint->m_localMatrix;

			// Matrix localMatrixBlender = joint->m_parent->m_worldMatrix * joint->m_localMatrix;
			// joint->m_offsetMatrix = localMatrixBlender.inverse();

			// joint->m_offsetMatrix = ( joint->m_parent->m_worldMatrix * joint->m_localMatrix ).inverse();
			// joint->m_offsetMatrix = joint->m_worldMatrix.inverse();
        }

        joint->m_offsetMatrix = joint->m_worldMatrix.inverse();

		// for ( SkeletalJoint* parent = joint->m_parent; parent != nullptr; parent = parent->m_parent )
		// {
		// 	joint->m_offsetMatrix = parent->m_offsetMatrix * joint->m_offsetMatrix;
		// }
	}

	// ---

	file.close();
	
	m_skeletons.insert( std::make_pair( filepath, skeleton ) );
    
    return skeleton;
}

Skeleton* ResourceManager::getSkeletonJson(const std::string& filepath)
{
	// Try cache ----------------------------------------------

	auto it = m_skeletons.find(filepath);

	if (it != m_skeletons.end())
	{
		// Msg( "skeleton '%s' has been retrieved from the cache", filepath.c_str() );
		return it->second;
	}

	// Load resource ------------------------------------------

	std::string path = m_assetsRootDirectory + filepath;

	fs::path filePath( path );
	// filePath.make_preferred();

	std::string pathString = filePath.string();
	std::replace( pathString.begin(), pathString.end(), '\\', '/' );

	Msg( "loading '%s'...", pathString.c_str() );

	std::ifstream inputFile( pathString );

	nlohmann::json json;
	inputFile >> json;

	nlohmann::json joints = json["joints"];

	Skeleton* skeleton = new Skeleton();

	int32_t numJoints = joints.size();

	// fix it, may not here
	skeleton->m_jointMatrices.resize(numJoints);

	std::map<int32_t, SkeletalJoint*> jointIndexToJoint;
	std::map<SkeletalJoint*, int32_t> jointToParentIndex;

	for ( const auto& joint_json : joints )
	{
		SkeletalJoint* joint = new SkeletalJoint();

		int jointIndex = joint_json["index"];
		jointIndexToJoint[jointIndex] = joint;

		// joint name
		std::string jointName = joint_json["name"];
		joint->m_name = jointName;

		// parent index
		int parentIndex = joint_json["parent_index"];
		jointToParentIndex[joint] = parentIndex;

		// std::string parentName = joint_json["parent_name"];

		// inverse bind pose matrix

		std::vector<float> inverseBindMatrix = joint_json["inverse_bind_matrix"];

		joint->m_inverseBindMatrix[0][0] = inverseBindMatrix[0];
		joint->m_inverseBindMatrix[0][1] = inverseBindMatrix[1];
		joint->m_inverseBindMatrix[0][2] = inverseBindMatrix[2];
		joint->m_inverseBindMatrix[0][3] = inverseBindMatrix[3];

		joint->m_inverseBindMatrix[1][0] = inverseBindMatrix[4];
		joint->m_inverseBindMatrix[1][1] = inverseBindMatrix[5];
		joint->m_inverseBindMatrix[1][2] = inverseBindMatrix[6];
		joint->m_inverseBindMatrix[1][3] = inverseBindMatrix[7];

		joint->m_inverseBindMatrix[2][0] = inverseBindMatrix[8];
		joint->m_inverseBindMatrix[2][1] = inverseBindMatrix[9];
		joint->m_inverseBindMatrix[2][2] = inverseBindMatrix[10];
		joint->m_inverseBindMatrix[2][3] = inverseBindMatrix[11];

		joint->m_inverseBindMatrix[3][0] = inverseBindMatrix[12];
		joint->m_inverseBindMatrix[3][1] = inverseBindMatrix[13];
		joint->m_inverseBindMatrix[3][2] = inverseBindMatrix[14];
		joint->m_inverseBindMatrix[3][3] = inverseBindMatrix[15];

		// joint->m_inverseBindMatrix = joint->m_inverseBindMatrix.transposed();

		// position

		std::vector<float> position = joint_json["position"];

		joint->m_position.x = position[0];
		joint->m_position.y = position[1];
		joint->m_position.z = position[2];

		// rotation

		std::vector<float> rotation = joint_json["rotation"];

		float x = rotation[0];
		float y = rotation[1];
		float z = rotation[2];
		float w = 0.0f;

		float t = 1.0f - (x * x) - (y * y) - (z * z);

		if ( t >= 0.0f )
		{
			// w = -sqrtf( t );
			w = sqrtf(t);
		}

		joint->m_rotation.x = x;
		joint->m_rotation.y = y;
		joint->m_rotation.z = z;
		joint->m_rotation.w = w;

		// ---

		skeleton->m_joints.push_back( joint );
	}

	for ( auto& joint : skeleton->m_joints )
	{
		int parentIndex = jointToParentIndex[joint];

		if ( parentIndex >= 0 )
		{
			SkeletalJoint* parentJoint = jointIndexToJoint[parentIndex];
			parentJoint->m_children.push_back( joint );

			joint->m_parent = parentJoint;
		}
	}

	for ( auto& joint : skeleton->m_joints )
	{
		joint->m_localMatrix = TransformMatrix( joint->m_position, joint->m_rotation );
	}

	SkeletalJoint* root = jointIndexToJoint[0];
	root->calcInverseBindTransform( Matrix::Identity );

	skeleton->m_root = root;
	
	m_skeletons.insert( std::make_pair( filepath, skeleton ) );
    
    return skeleton;
}

Animation* ResourceManager::getAnimSequence( const std::string& filepath )
{
	// Try cache ----------------------------------------------

    auto it = m_animations.find( filepath );
    
	if ( it != m_animations.end() )
	{
		// Msg( "animation '%s' has been retrieved from the cache", filepath.c_str() );
		return it->second;
	}

	// Load resource ------------------------------------------

	std::string path = m_assetsRootDirectory + filepath;
	Msg( "loading '%s'...", filepath.c_str() );

	std::ifstream file( path, std::ofstream::binary );

	if ( file.is_open() == false )
	{
		Warning( "unable to open '%s'! skipping...", path.c_str() );
		return nullptr;
	}

	Animation* animation = new Animation();

	// animation name

	uint8_t animationNameLength = 0;
	read( file, &animationNameLength );

	std::string animationName;
	animationName.resize( animationNameLength );
	read( file, &animationName[0], animationNameLength );

	animation->m_name = animationName;

	// frame rate

	read( file, &animation->m_framerate );

	// duration

	read( file, &animation->m_numFrames );

	// channels

	int32_t numChannels = 0;
	read( file, &numChannels );

	for ( int32_t i = 0; i < numChannels; ++i )
	{
		JointAnimChannel channel = {};

		// channel name

		uint8_t jointNameLength = 0;
		read( file, &jointNameLength );

		std::string jointName;
		jointName.resize( jointNameLength );
		read( file, &jointName[0], jointNameLength );

		channel.m_jointName = jointName;

		// position keyframe

		int32_t numPositionKeys = 0;
		read( file, &numPositionKeys );

		for ( int32_t i = 0; i < numPositionKeys; ++i )
		{
			VectorKeyframe positionKey = {};
			read( file, &positionKey );

			channel.m_positionKeyframes.push_back( positionKey );
		}

		// rotation keyframe

		int32_t numRotationKeys = 0;
		read( file, &numRotationKeys );

		for ( int32_t i = 0; i < numRotationKeys; ++i )
		{
			QuaternionKeyframe rotationKey = {};
			read( file, &rotationKey );

			channel.m_rotationKeyframes.push_back( rotationKey );
		}

		// position keyframe

		int32_t numScalingKeys = 0;
		read( file, &numScalingKeys );

		for ( int32_t i = 0; i < numScalingKeys; ++i )
		{
			VectorKeyframe scalingKey = {};
			read( file, &scalingKey );

			channel.m_scaleKeyframes.push_back( scalingKey );
		}

		animation->m_channels.push_back( channel );
	}

	file.close();
	
	m_animations.insert( std::make_pair( filepath, animation ) );
    
    return animation;
}

Animation* ResourceManager::getAnimSequenceJson( const std::string& filepath )
{
	// Try cache ----------------------------------------------

    auto it = m_animations.find( filepath );
    
	if ( it != m_animations.end() )
	{
		// Msg( "animation '%s' has been retrieved from the cache", filepath.c_str() );
		return it->second;
	}

	// Load resource ------------------------------------------

	std::string path = m_assetsRootDirectory + filepath;
	std::replace( path.begin(), path.end(), '\\', '/' );

	Msg( "loading '%s'...", path.c_str() );

	std::ifstream inputFile( path );

	nlohmann::json json;
	inputFile >> json;


	std::string animationName = json["name"];
	float time = json["time"];
	int32_t numFrames = json["num_frames"];
	int8_t framerate = json["framerate"];

	nlohmann::json channels = json["channels"];

	Animation* animation = new Animation();

	// animation

	animation->m_name = animationName;
	animation->m_numFrames = numFrames;
	animation->m_framerate = framerate;

	// channels

	int32_t numChannels = channels.size();

	for ( const auto& channel_json : channels )
	{
		JointAnimChannel channel = {};

		std::string jointName = channel_json["joint_name"];

		channel.m_jointName = jointName;

		nlohmann::json position_keyframes = channel_json["position_keyframes"];
		nlohmann::json rotation_keyframes = channel_json["rotation_keyframes"];

		// position keyframe

		for ( const auto& pos_kf_json : position_keyframes )
		{
			VectorKeyframe positionKey = {};

			float time = pos_kf_json["time"];
			float x = pos_kf_json["x"];
			float y = pos_kf_json["y"];
			float z = pos_kf_json["z"];

			positionKey.m_time = time;
			positionKey.m_value = Vector( x, y, z );

			channel.m_positionKeyframes.push_back( positionKey );
		}

		// rotation keyframe

		for ( const auto& rot_kf_json : rotation_keyframes )
		{
			QuaternionKeyframe rotationKey = {};

			float time = rot_kf_json["time"];
			float x = rot_kf_json["x"];
			float y = rot_kf_json["y"];
			float z = rot_kf_json["z"];
			float w = rot_kf_json["w"];

			rotationKey.m_time = time;
			rotationKey.m_value = Quaternion( x, y, z, w );

			channel.m_rotationKeyframes.push_back( rotationKey );
		}

		animation->m_channels.push_back( channel );
	}
	
	m_animations.insert( std::make_pair( filepath, animation ) );
    
    return animation;
}

StaticMesh* ResourceManager::getStaticMesh( const std::string& filename )
{
	// Try cache ----------------------------------------------

    auto it = m_meshes.find( filename );
    
	if ( it != m_meshes.end() )
	{
		// Msg( "static mesh '%s' has been retrieved from the cache", filename.c_str() );
		return it->second;
	}

	// Load resource ------------------------------------------

	std::string filepath = m_assetsRootDirectory + filename;
	Msg( "loading '%s'...", filename.c_str() );
	
	std::ifstream file;
	file.open( filepath, std::ios::binary );
	
	if ( file.is_open() == false )
	{
		Warning( "unable to open '%s'! skipping...", filepath.c_str() );
		return nullptr;
	}
	
	StaticMesh* mesh = new StaticMesh();
	Surface* surface = new Surface();

	// vertices

	int32_t numVertices = 0;
	read( file, &numVertices );

	for ( int32_t i = 0; i < numVertices; ++i )
	{
		Vertex vertex = {};

		read( file, &vertex.position );
		read( file, &vertex.normal );
		read( file, &vertex.tangent );
		read( file, &vertex.uv );
		
		surface->addVertex( vertex );
	}

	// generate triangles data

	int32_t numTriangles = numVertices / 3;

	for ( int32_t i = 0; i < numTriangles; ++i )
	{
		Triangle triangle = {};

		triangle.m_vertexIndex[0] = i * 3;
		triangle.m_vertexIndex[1] = i * 3 + 1;
		triangle.m_vertexIndex[2] = i * 3 + 2;
			
		surface->addTriangle( triangle );
	}

	mesh->addSurface( surface );

	mesh->flushMesh();

	file.close();
	
	m_meshes.insert( std::make_pair( filename, mesh ) );
    
    return mesh;
}

struct VertexWeight
{
	int32_t jointIndex;
	float jointWeight;
};

StaticMesh* ResourceManager::getMesh( const std::string& filename )
{
	// Try cache ----------------------------------------------

    auto it = m_meshes.find( filename );
    
	if ( it != m_meshes.end() )
	{
		// Msg( "static mesh '%s' has been retrieved from the cache", filename.c_str() );
		return it->second;
	}

	// Load resource ------------------------------------------

	std::string path = m_assetsRootDirectory + filename;
	std::replace( path.begin(), path.end(), '\\', '/' );

	Msg( "loading '%s'...", path.c_str() );

	std::ifstream file( path );
	
	if ( !file.is_open() )
	{
		Warning( "unable to open '%s'! skipping...", path.c_str() );
		return nullptr;
	}

	nlohmann::json json;
	file >> json;

	nlohmann::json submeshes = json["submeshes"];
	
	StaticMesh* mesh = new StaticMesh();
	
	int32_t numSurfaces = submeshes.size();

	int32_t i = 0;
	for ( const auto & submesh_json : submeshes )
	{
		Surface* surface = new Surface();

		// surface name

		std::string submeshName = submesh_json["name"];
		surface->setName( submeshName );

		// surface material

		surface->setMaterial( "materials/default.mat" );

		std::string materialPath = submesh_json["material"];

		if ( !materialPath.empty() )
		{
			surface->setMaterial( materialPath );
		}

		// arrays of indices data
		std::vector<float> vertex_positions = submesh_json["positions"];
		std::vector<float> vertex_normals = submesh_json["normals"];
		std::vector<float> vertex_texcoords = submesh_json["uvs"];

		std::vector<int> triangles = submesh_json["triangles"];

		std::vector<nlohmann::json> skinning = submesh_json["skinning"];

		// skinning

		std::unordered_map<int32_t, std::vector<VertexWeight>> weightsMap; // vertexIndexToWeights

		for ( const auto& item : skinning )
		{
			int vertex_index = item["vertex_index"];

			VertexWeight vertexWeight = {};
			vertexWeight.jointIndex = item["joint_index"];
			vertexWeight.jointWeight = item["joint_weight"];

			auto it = weightsMap.find( vertex_index );

			if ( it != weightsMap.end() )
			{
				it->second.push_back( vertexWeight );
			}
			else
			{
				weightsMap[vertex_index] = { vertexWeight };
			}
		}

		int triangleIndicesCount = triangles.size();
		int numTriangles = triangles.size() / 9;
		int numVertices = triangles.size() / 3;

		int positionCount = vertex_positions.size();
		int skinningCount = skinning.size();
		int skinningCount2 = weightsMap.size();

		// vertices

		std::vector<Vertex> vertices;

		for ( int32_t i = 0; i < triangleIndicesCount; i += 3 )
		{
			// i - vertexIndex
			// vertexIndexToWeights[vertexIndex]

			Vertex vertex = {};

			int32_t p_idx = triangles[i+0];
			int32_t t_idx = triangles[i+1];
			int32_t n_idx = triangles[i+2];

			// assert( p_idx < skinningCount2 );

			vertex.position[0] = vertex_positions[p_idx * 3 + 0];
			vertex.position[1] = vertex_positions[p_idx * 3 + 1];
			vertex.position[2] = vertex_positions[p_idx * 3 + 2];

			vertex.normal[0] = vertex_normals[n_idx * 3 + 0];
			vertex.normal[1] = vertex_normals[n_idx * 3 + 1];
			vertex.normal[2] = vertex_normals[n_idx * 3 + 2];

			vertex.uv[0] = vertex_texcoords[t_idx * 2 + 0];
			vertex.uv[1] = vertex_texcoords[t_idx * 2 + 1];

			if ( vertices.size() == 1234 )
			{
				printf("");
			}

			auto it = weightsMap.find( p_idx );

			if ( it != weightsMap.end() )
			{
				for ( const auto& vertexWeight : it->second )
				{
					vertex.addJointInfluenceInfo( vertexWeight.jointIndex, vertexWeight.jointWeight );
				}
			}

			vertices.push_back( vertex );
			// surface->addVertex( vertex );
		}

		// triangles

		for ( int32_t j = 0; j < numVertices; j += 3 )
		{
			Triangle triangle = {};
			
			triangle.m_vertexIndex[0] = j+0;
			triangle.m_vertexIndex[1] = j+1;
			triangle.m_vertexIndex[2] = j+2;
			
			surface->addTriangle( triangle );
		}

		

		// weightsMap.insert( std::make_pair( filename, mesh ) );

		/*for ( const auto& item : skinning )
		{
			int vertex_index = item["vertex_index"];
			int joint_index = item["joint_index"];
			float joint_weight = item["joint_weight"];

			vertices[vertex_index].addJointInfluenceInfo( joint_index, joint_weight );
		}*/

		for ( const auto& vertex : vertices )
		{
			surface->addVertex( vertex );
		}

		mesh->addSurface( surface );
	}

	mesh->flushMesh();

	m_meshes.insert( std::make_pair( filename, mesh ) );
    
    return mesh;
}

StaticMesh* ResourceManager::getStaticMesh2( const std::string& filename )
{
	// Try cache ----------------------------------------------

    auto it = m_meshes.find( filename );
    
	if ( it != m_meshes.end() )
	{
		// Msg( "static mesh '%s' has been retrieved from the cache", filename.c_str() );
		return it->second;
	}

	// Load resource ------------------------------------------

	std::string filepath = m_assetsRootDirectory + filename;
	Msg( "loading '%s'...", filename.c_str() );
	
	std::ifstream file;
	file.open( filepath, std::ios::binary );
	
	if ( file.is_open() == false )
	{
		Warning( "unable to open '%s'! skipping...", filepath.c_str() );
		return nullptr;
	}
	
	StaticMesh* mesh = new StaticMesh();
	
	int32_t numSurfaces = 0;
	read( file, &numSurfaces );

	for ( int32_t i = 0; i < numSurfaces; ++i )
	{
		Surface* surface = new Surface();

		// surface name

		uint8_t surfaceNameLength = 0;
		read( file, &surfaceNameLength );

		std::string surfaceName;
		surfaceName.resize( surfaceNameLength );
		read( file, &surfaceName[0], surfaceNameLength );

		surface->setName( surfaceName );

		// surface material

		uint8_t materialPathLength = 0;
		read( file, &materialPathLength );

		std::string materialPath;
		materialPath.resize( materialPathLength );
		read( file, &materialPath[0], materialPathLength );

		Material* material = getMaterial( materialPath );
		surface->setMaterial( material );

		// vertices

		int32_t numVertices = 0;
		read( file, &numVertices );

		for ( int32_t j = 0; j < numVertices; ++j )
		{
			Vertex vertex = {};
			read( file, &vertex );
		
			surface->addVertex( vertex );
		}

		// triangles

		int32_t numTriangles = 0;
		read( file, &numTriangles );

		for ( int32_t j = 0; j < numTriangles; ++j )
		{
			Triangle triangle = {};
			read( file, &triangle );
			
			surface->addTriangle( triangle );
		}

		mesh->addSurface( surface );
	}

	mesh->flushMesh();

	file.close();
	
	m_meshes.insert( std::make_pair( filename, mesh ) );
    
    return mesh;
}

Shader* ResourceManager::getShader( const std::string& filename )
{	
	// Try cache ----------------------------------------------

	auto it = m_shaders.find( filename );

	if ( it != m_shaders.end() )
    {
        // Msg( "shader '%s' has been retrieved from the cache", file.c_str() );
        return it->second;
    }

	// Load resource ------------------------------------------
		
	std::string path = m_assetsRootDirectory + filename;
	std::replace( path.begin(), path.end(), '\\', '/' );

	Msg( "loading '%s'...", filename.c_str() );

	Shader* shader = new Shader();

	std::string vertexSrc = Filesystem.readFileData( path + ".vert" );
	shader->setVertexSource( vertexSrc );

	std::string fragmentSrc = Filesystem.readFileData( path + ".frag" );
	shader->setFragmentSource( fragmentSrc );

	if ( !shader->compile() )
	{
		SafeDelete( shader );
		return nullptr;
	}

	shader->debugName = filename;

	m_shaders.insert( std::make_pair( filename, shader ) );
    
    return shader;
}

bool startsWith(const char* base, const char* str) 
{
	char c;
	while ((c = *str++) != '\0')
		if (c != *base++) return false;
	return true;
}

bool endsWith(const char* base, const char* str)
{
	int slen = strlen(str) - 1;
	const char* p = base + strlen(base) - 1;
	while (p > base && isspace(*p)) p--;  // rtrim
	p -= slen;
	if (p < base) return false;
	return (strncmp(p, str, slen) == 0);
}

Texture* ResourceManager::getTexture( const std::string& filename )
{    
	auto it = m_textures.find( filename );

	if ( it != m_textures.end() )
	{
		// Msg( "texture '%s' has been retrieved from the cache", filename.c_str() );
		return it->second;
	}
	
	std::string filepath = m_assetsRootDirectory + filename;
	Msg( "loading '%s'...", filename.c_str() );
	
	Texture* texture = nullptr;
	GLuint textureId = 0;

	if ( endsWith( filename.c_str(), ".dds" ) )
	{
		textureId = SOIL_load_OGL_texture
		(
			filepath.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_DDS_LOAD_DIRECT
		);

		if ( textureId == 0 )
		{
			Warning( "unable to open '%s'! skipping...", filepath.c_str() );
			return nullptr;
		}

		texture = new Texture();

		glBindTexture( GL_TEXTURE_2D, textureId );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16 );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		//GLint levels;
		//glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, &levels );

		//GLint baseLevel;
		//glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, &baseLevel );

		// Msg( "levels: %i", levels );
		// Msg( "baseLevel: %i", baseLevel );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	else if ( endsWith( filename.c_str(), ".hdr" ) )
	{
		//textureId = SOIL_load_OGL_HDR_texture
		//(
		//	filepath.c_str(),
		//	SOIL_HDR_RGBE,
		//	0,
		//	SOIL_CREATE_NEW_ID,
		//	0
		//);

		//if ( textureId == 0 )
		//{
		//	Warning( "unable to open '%s'! skipping...", filepath.c_str() );
		//	return nullptr;
		//}

		//texture = new Texture();

		//glBindTexture( GL_TEXTURE_2D, textureId );

		//// glGenerateMipmap( GL_TEXTURE_2D );

		////glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16 );

		//// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		//glBindTexture( GL_TEXTURE_2D, 0 );

		// ---

		stbi_set_flip_vertically_on_load( true );

		int width, height, numChannels;
		float* data = stbi_loadf( filepath.c_str(), &width, &height, &numChannels, 0 );

		if ( !data )
		{
			Warning( "unable to open '%s'! skipping...", filepath.c_str() );
			return nullptr;
		}

		texture = new Texture();

		texture->m_width = width;
		texture->m_height = height;

		glGenTextures( 1, &textureId );
		glBindTexture( GL_TEXTURE_2D, textureId );
		
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glBindTexture( GL_TEXTURE_2D, 0 );

		stbi_image_free( data );
		stbi_set_flip_vertically_on_load( false );
	}
	else
	{
		int32_t width, height, numChannels;
		unsigned char* data = SOIL_load_image( filepath.c_str(), &width, &height, &numChannels, SOIL_LOAD_AUTO );
		
		if ( !data )
		{
			Warning( "unable to open '%s'! skipping...", filepath.c_str() );
			return nullptr;
		}

		texture = new Texture();

		texture->m_width = width;
		texture->m_height = height;

		glGenTextures( 1, &textureId );
		glBindTexture( GL_TEXTURE_2D, textureId );
		
		//Msg( "%ix%i", width, height );
		//Msg( "textureId: %i", textureHandle );

		GLenum format = 0;

		if ( numChannels == 1 )
		{
			format = GL_RED; // 6403
			texture->m_numChannels = 1;
		}

		else if ( numChannels == 3 )
		{
			format = GL_RGB; // 6407
			texture->m_numChannels = 3;
		}

		else if ( numChannels == 4 )
		{
			format = GL_RGBA; // 6408
			texture->m_numChannels = 4;
		}

		glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
		// glGenerateMipmap( GL_TEXTURE_2D );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glBindTexture( GL_TEXTURE_2D, 0 );
		SOIL_free_image_data( data );
	}

	texture->setTextureId( textureId );
	
	m_textures.insert( std::make_pair( filename, texture ) );

    return texture;
}

Material* ResourceManager::createMaterial( const std::string& name, Shader* shader /* = nullptr */ )
{    
    auto it = m_materials.find( name );
    
	// TODO: this is new material, cache have not instance

	if ( it != m_materials.end() )
	{
		// Msg( "material '%s' has been retrieved from the cache", name.c_str() );
		return it->second;
	}

	Material* material = new Material( name, shader );
	m_materials.insert( std::make_pair( name, material ) );
    
    return material;
}

Material* ResourceManager::getMaterial( const std::string& filename )
{    
    auto it = m_materials.find( filename );
    
    if ( it != m_materials.end() )
    {
		// Msg( "material '%s' has been retrieved from the cache", filename.c_str() );
		return it->second;
	}
	
	std::string filepath = m_assetsRootDirectory + filename;

	filepath = sanitize_path( filepath );

	Msg( "loading '%s'...", filename.c_str() );
	
	MaterialLoader loader;
	Material* material = loader.load( filepath );

	if ( material )
	{
		m_materials.insert( std::make_pair( filename, material ) );
	}

	return material;
}

Sound* ResourceManager::getSound( const std::string& file )
{    
    auto it = m_sounds.find( file );
    
	if ( it != m_sounds.end() )
    {
		// Msg( "sound '%s' has been retrieved from the cache", file.c_str() );
		return it->second;
	}
	
	Sound* sound = new Sound();
	
	std::string path = m_assetsRootDirectory + file;
	Msg( "loading '%s'...", file.c_str() );
	
	SDL_AudioSpec specification;
	uint8_t* buffer;
	uint32_t length;
	
	bool status = SDL_LoadWAV( path.c_str(), &specification, &buffer, &length );

	if ( status == false )
	{
		Warning( "unable to open '%s'! skipping...", path.c_str() );
		return nullptr;
	}

	int dataFormat = 0;

	if ( specification.channels == 2 && specification.format == AUDIO_S16 )
	{
		dataFormat = AL_FORMAT_STEREO16;
	}
	else if ( specification.channels == 1 && specification.format == AUDIO_S16 )
	{
		dataFormat = AL_FORMAT_MONO16;
	}
	else
	{
		Warning( "unknown audio data format" );
	}
	
	alBufferData( sound->getSoundId(), dataFormat, buffer, length, specification.freq );
	
	if ( alGetError() != 0 )
	{
		Warning( "something went wrong" );
		return nullptr;
	}

	m_sounds.insert( std::make_pair( file, sound ) );
	
	return sound;
}

