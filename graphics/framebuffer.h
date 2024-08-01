// Oleg Kotov

#include "okobase.h"
#include <gl/glew.h>

enum DepthBufferType
{
	DepthBufferType_None,
	DepthBufferType_Texture,
	DepthBufferType_Renderbuffer
};

class ENGINE_API Framebuffer
{
public:

	Framebuffer( uint16 width, uint16 height, DepthBufferType depthBufferType )
	{
		m_width = width;
		m_height = height;

		initFramebuffer( depthBufferType );
	}

	~Framebuffer()
	{
		glDeleteFramebuffers( m_framebuffer );
		glDeleteTextures( m_colorTexture );
		glDeleteTextures( m_depthTexture );
		glDeleteRenderbuffers( m_depthBuffer );
	}

	void initFramebuffer( DepthBufferType type )
	{
		createFramebuffer();
		createTextureAttachment();

		if ( type == DepthBufferType_Renderbuffer )
		{
			createDepthBufferAttachment();
		}
		else if ( type == DepthBufferType_Texture )
		{
			createDepthTextureAttachment();
		}

		if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
		{
			Warning( "framebuffer is not complete!" );
		}

		unbindFramebuffer();
	}

	void createFramebuffer()
	{
		glGenFramebuffers( 1, &m_framebuffer );
		glBindFramebuffer( GL_FRAMEBUFFER, m_framebuffer );
	}

	void createTextureAttachment()
	{
		glGenTextures( 1, &m_colorTexture );
		glBindTexture( GL_TEXTURE_2D, m_colorTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glBindTexture( GL_TEXTURE_2D, 0 );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0 );
	}

	void createDepthBufferAttachment()
	{
		glGenRenderbuffers( 1, &m_depthBuffer );
		glBindRenderbuffer( GL_RENDERBUFFER, m_depthBuffer );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height );
		glBindRenderbuffer( GL_RENDERBUFFER, 0 );

		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer );
	}

	void createDepthTextureAttachment()
	{
		glGenTextures( 1, &m_depthTexture );
		glBindTexture( GL_TEXTURE_2D, m_depthTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glBindTexture( GL_TEXTURE_2D, 0 );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0 );
	}

	void bindFramebuffer()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, m_framebuffer );
		glViewport( 0, 0, m_width, m_height );
	}

	void unbindFramebuffer()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		// glViewport( 0, 0, m_width, m_height ); win.width, win.height
	}

private:

	uint16 m_width = 0;
	uint16 m_height = 0;

	uint32 m_framebuffer = 0;

	uint32 m_colorTexture = 0;
	uint32 m_depthTexture = 0;
	uint32 m_depthBuffer = 0;
};

