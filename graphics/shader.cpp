// Oleg Kotov

#include "shader.h"
#include "vector.h"
#include "color.h"
#include "matrix.h"
#include "log.h"

#include <GL/glew.h>

Shader::Shader()
{
    m_shaderProgram = glCreateProgram();
    m_vertexShader = glCreateShader( GL_VERTEX_SHADER );
    m_fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
}

Shader::~Shader()
{
	glDeleteProgram( m_shaderProgram );
}

void Shader::setVertexSource( const std::string& code ) const
{
	const char* src = code.c_str();
	glShaderSource( m_vertexShader, 1, &src, nullptr );
}

void Shader::setFragmentSource( const std::string& code ) const
{
	const char* src = code.c_str();
	glShaderSource( m_fragmentShader, 1, &src, nullptr );
}

bool Shader::compile()
{
    if ( !compileShader( m_vertexShader ) ) return false;
    if ( !compileShader( m_fragmentShader ) ) return false;

    glAttachShader( m_shaderProgram, m_vertexShader );
    glAttachShader( m_shaderProgram, m_fragmentShader );

    if ( !linkShaderProgram( m_shaderProgram ) ) return false;
    
    glDeleteShader( m_vertexShader );
    glDeleteShader( m_fragmentShader );

    return true;
}

void Shader::bind() const
{
    glUseProgram( m_shaderProgram );
}

void Shader::unbind() const
{
    glUseProgram( 0 );
}

void Shader::setIntegerUniform( const std::string& name, int32_t value ) const
{
    glUniform1i( glGetUniformLocation( m_shaderProgram, name.c_str() ), value );
}

void Shader::setFloatUniform( const std::string& name, float value ) const
{
    glUniform1f( glGetUniformLocation( m_shaderProgram, name.c_str() ), value);
}

void Shader::setVectorUniform( const std::string& name, const Vector& value ) const
{
    glUniform3f( glGetUniformLocation( m_shaderProgram, name.c_str() ), value.x, value.y, value.z );
}

void Shader::setMatrixUniform( const std::string& name, const Matrix& value ) const
{
    glUniformMatrix4fv( glGetUniformLocation( m_shaderProgram, name.c_str() ), 1, GL_FALSE, &value[0][0] );
}

void Shader::setColorUniform( const std::string& name, const Color& value ) const
{
    glUniform4f( glGetUniformLocation( m_shaderProgram, name.c_str() ), value.r, value.g, value.b, value.a );
}

void Shader::setBoolUniform( const std::string& name, bool value ) const
{
	glUniform1i( glGetUniformLocation( m_shaderProgram, name.c_str() ), ( value == true ) ? 1 : 0 );
}

bool Shader::compileShader( uint32_t shader )
{
    glCompileShader( shader );

    GLint status = 0;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status );

    if ( status == GL_FALSE )
    {
        GLint logLength = 0;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );

        if ( logLength > 0 )
        {
            char* log = new char[logLength];

            GLsizei written;
            glGetShaderInfoLog( shader, logLength, &written, log );

            Warning( "shader compilation failed:\n%s", log );
            delete[] log;
            return false;
        }

        Warning( "shader compilation failed" );
        return false;
    }

    return true;
}

bool Shader::linkShaderProgram( uint32_t program )
{
    glLinkProgram( m_shaderProgram );
    glValidateProgram( m_shaderProgram );

    GLint status = 0;
    glGetProgramiv( program, GL_LINK_STATUS, &status );

    if ( status == GL_FALSE )
    {
        GLint logLength = 0;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );

        if ( logLength > 0 )
        {
            char* log = new char[logLength];

            GLsizei written;
            glGetProgramInfoLog( program, logLength, &written, log );

            Warning( "failed to link shader program:\n%s", log );
            delete[] log;
            return false;
        }

        Warning( "failed to link shader program" );
        return false;
    }

    return true;
}

