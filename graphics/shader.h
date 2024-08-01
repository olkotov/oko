// Oleg Kotov

#pragma once

#include "baseapi.h"

#include <stdint.h>
#include <string>

class Vector;
class Matrix;
class Color;

class ENGINE_API Shader
{
public:

    Shader();
    ~Shader();
    
    void setVertexSource( const std::string& code ) const;
    void setFragmentSource( const std::string& code ) const;
    bool compile();

    void bind() const;
    void unbind() const;

    void setIntegerUniform( const std::string& name, int32_t value ) const;
    void setFloatUniform( const std::string& name, float value ) const;
    void setVectorUniform( const std::string& name, const Vector& value ) const;
    void setMatrixUniform( const std::string& name, const Matrix& value ) const;
    void setColorUniform( const std::string& name, const Color& value ) const;
    void setBoolUniform( const std::string& name, bool value ) const;

    uint32_t getNativeHandle() const { return m_shaderProgram; }

private:

    bool compileShader( uint32_t shader );
    bool linkShaderProgram( uint32_t program );

private:

	uint32_t m_shaderProgram = 0;
    uint32_t m_vertexShader = 0;
    uint32_t m_fragmentShader = 0;

    // std::map<> m_attributeLocations;

public:

    std::string debugName;
};

