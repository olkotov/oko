// Oleg Kotov

#pragma once

#include "okobase.h"

class  Material;
struct Vertex;
struct Triangle;
class Shader;

/**
* Represents one surface of the mesh.
***********************************************************************************/

class Surface
{
public:

    Surface();
    ~Surface();

	void setName( const std::string& name );
	const std::string& getName() const;

	void setMaterial( Material* material );
	void setMaterial( const std::string& filename );
	Material* getMaterial() const;

	void addVertex( const Vertex& vertex );
	const std::vector<Vertex>& getVertices() const;
	int32_t getNumVertices() const;

	void addTriangle( const Triangle& triangle );
	const std::vector<Triangle>& getTriangles() const;
	int32_t getNumTriangles() const;

	int32_t getNumIndices() const;

	uint32_t getVertexArray() const;

    void flushSurface();

	void bindToShader( Shader* shader ) const;

private:

	std::string m_name;
	Material* m_material = nullptr;

	int32_t m_numVertices = 0;
	std::vector<Vertex> m_vertices;

	int32_t m_numTriangles = 0;
	std::vector<Triangle> m_triangles;

	int32_t m_numIndices = 0;

	uint32_t m_vertexArray = 0;
	uint32_t m_vertexBuffer = 0;
	uint32_t m_indexBuffer = 0;
};

