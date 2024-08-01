// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "vector.h"
#include "color.h"

#include <stdint.h>
#include <assert.h>

#define JOINTS_PER_VERTEX 4

//struct MeshVertex
//{
//    float m_position[3];
//    float m_normal[3];
//    float m_tangent[3];
//    float m_textureCoords[2];
//};

struct ENGINE_API Vertex
{
	void addJointInfluenceInfo( int32_t inJointIndex, float inJointWeight )
    {
        for ( int32_t i = 0; i < JOINTS_PER_VERTEX; ++i )
        {
            if ( jointWeight[i] == 0.0f ) 
            {
                jointIndex[i]  = inJointIndex;
                jointWeight[i] = inJointWeight;

                return;
            }
        }

        // should never get here - more joint than we have space for
        assert( 0 );
    }

	float    position[3] = {};
    float    normal[3] = {};
    float    tangent[3] = {};
    float    uv[2] = {};

    // vertex skinning data
	uint32_t jointIndex[JOINTS_PER_VERTEX] = { 0 };
	float    jointWeight[JOINTS_PER_VERTEX] = { 0.0f };
};

//struct SkinnedVertex
//{
//    Vector position;
//    // float  texcoord[2];
//    // Color  color;
//    // Vector normal;
//    // Vector tangent;
//    // Vector bitangent;
//    // uint8  influenceJoints[4];
//    int32 influenceJoints[4];
//    float influenceWeights[4];
//};

//struct Vertex
//{
//    float position[3];
//    float normal[3];
//    float uv[2];
//    
//    uint32 color;
//    
//    uint8 indices[4];
//    float weights[4];
//    
//    float tangent[3];
//    float binormal[3];
//};

struct DebugVertex
{
	DebugVertex( const Vector& position, const Color& color )
		: position( position )
		, color( color )
	{}

	Vector position;
	Color  color;
};

// tintColor

