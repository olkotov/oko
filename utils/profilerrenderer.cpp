// Oleg Kotov

#include "profilerrenderer.h"
#include "textelement.h"
#include "shader.h"
#include "okoengine.h"
#include "window.h"
#include "resourcemanager.h"
#include "graphics.h"
#include "font.h"
#include "texture.h"
#include "profiler.h"
#include "log.h"

#include <GL/glew.h>

ProfilerRenderer::ProfilerRenderer()
{
	Msg( "profilerrenderer:initialize" );

	m_textShader = engine()->resources()->getShader( "shaders/text_sdf" );

	m_textElement = new TextElement();
	m_textElement->setPosition( 5, 10 );
	m_textElement->setFont( "fonts/consolas.fnt" );
	m_textElement->setFontScale( 0.25f );
	m_textElement->setColor( Color::Green );
	m_textElement->setUseShadow( true );
}

ProfilerRenderer::~ProfilerRenderer()
{
	Msg( "profilerrenderer:shutdown" );
	SafeDelete( m_textElement );
}

void ProfilerRenderer::draw( Profiler* profiler )
{
	if ( !profiler || !m_textShader ) return;

	{
		auto stats = profiler->getStats();

		std::string text;

		for ( const auto& stat : stats )
		{
			char statText[255];
			sprintf( statText, "[%15s]: %.2f%s\n", stat.name.c_str(), stat.value, stat.units.c_str() );

			text.append( statText );
		}

		m_textElement->setText( text );
	}

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_DEPTH_TEST );

	{
		m_textShader->bind();
		m_textElement->bindToShader( m_textShader );

		static const int8_t textureSlot = 0;
		glActiveTexture( GL_TEXTURE0 + textureSlot );
		glBindTexture( GL_TEXTURE_2D, m_textElement->getFont()->getTextureAtlas()->getTextureId() );
		m_textShader->setIntegerUniform( "_FontTextureAtlas", textureSlot );

		m_textShader->setColorUniform( "_Color", m_textElement->getColor() );
		m_textShader->setVectorUniform( "_Transform", m_textElement->getTransform() );
		m_textShader->setBoolUniform( "_UseShadow", m_textElement->isUseShadow() );

		uint16_t screenWidth, screenHeight;
		engine()->window()->getResolution( screenWidth, screenHeight );

		m_textShader->setVectorUniform( "_DisplaySize", Vector( screenWidth, screenHeight, 0.0f ) );

		glBindVertexArray( m_textElement->getVertexArray() );
		// glDrawArrays( GL_TRIANGLES, 0, m_textElement->getNumVertices() );
		// glBindVertexArray( 0 );
	}

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );
}

void ProfilerRenderer::clear()
{
	m_textElement->setText( "" );
}

