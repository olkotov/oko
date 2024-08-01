// Oleg Kotov

#pragma once

#include "primitivecomponent.h"
#include <stdint.h>

class StaticMesh;
class Material;

class ENGINE_API StaticMeshComponent : public PrimitiveComponent
{   
public:
    
    StaticMeshComponent();
    ~StaticMeshComponent();
        
    void setMesh( StaticMesh* mesh );
    void setMesh( const std::string& filename );
    StaticMesh* getMesh() const;

	void setMaterial( Material* material, int32_t surfaceIndex = 0 );
    void setMaterial( const std::string& filename, int32_t surfaceIndex = 0 );
	Material* getMaterial( int32_t surfaceIndex = 0 ) const;

    void createCollisionShape();

private:

	StaticMesh* m_mesh = nullptr;
	std::vector<Material*> m_materialOverrides;

    // physics
    class btTriangleMesh* m_physicsMesh = nullptr;
};

