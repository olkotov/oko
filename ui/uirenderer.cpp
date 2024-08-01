// Oleg Kotov

#include "uirenderer.h"
#include "shader.h"
#include "texture.h"
#include "okoengine.h"
#include "resourcemanager.h"
#include "graphics.h"
#include "cameracomponent.h"
#include "rectangleelement.h"
#include "spriteelement.h"
#include "textelement.h"
#include "transformmatrix.h"
#include "font.h"
#include "window.h"
#include "log.h"

#include <GL/glew.h>

UIRenderer::UIRenderer() :
	m_rectangleVertices
	{
	 0.0f, 0.0f,
	 0.0f, 1.0f,
	 1.0f, 0.0f,
	 1.0f, 1.0f
	}
{
	Msg( "uirenderer:initialize" );

	// rectangle vertices, text used own
	glGenVertexArrays( 1, &m_vertexArray );
	glGenBuffers( 1, &m_vertexBuffer );

	flushVertexData();

	m_rectangleShader = engine()->resources()->getShader( "shaders/ui_rectangle" );
	m_textShader = engine()->resources()->getShader( "shaders/text_sdf" );
}

UIRenderer::~UIRenderer()
{
	Msg( "uirenderer:shutdown" );

    glDeleteBuffers( 1, &m_vertexBuffer );
    glDeleteVertexArrays( 1, &m_vertexArray );
}

void UIRenderer::flushVertexData() const
{
	glBindVertexArray( m_vertexArray );
    
	glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( m_rectangleVertices ), &m_rectangleVertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void UIRenderer::bindVertexDataToShader() const
{
	glBindVertexArray( m_vertexArray );
    glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
    
    GLint positionAttribute = glGetAttribLocation( m_rectangleShader->getNativeHandle(), "position" );
	glEnableVertexAttribArray( positionAttribute );
	glVertexAttribPointer(
		/* index     = */ positionAttribute,
		/* size      = */ 2,
		/* type      = */ GL_FLOAT,
		/* normalize = */ GL_FALSE,
		/* stride    = */ 2 * sizeof( float ),
		/* offset    = */ (void*)0 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void UIRenderer::addText( TextElement* text )
{
	m_texts.push_back( text );
}

void UIRenderer::removeText( TextElement* text )
{
	// hmm... order matter?
	// or just rewrite it all?

	auto it = std::find( m_texts.begin(), m_texts.end(), text );

	if ( it != m_texts.end() )
	{
		std::iter_swap( it, m_texts.end() - 1 );
		m_texts.pop_back();
	}
}

void UIRenderer::addRectangle( RectangleElement* rect )
{
	m_rectangles.push_back( rect );
}

void UIRenderer::drawUI()
{
	if ( !m_drawEnabled || !m_rectangleShader || !m_textShader ) return;

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_DEPTH_TEST );

	drawRectangles();
	drawTextElements();

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );
}

void UIRenderer::drawRectangles() const
{
	m_rectangleShader->bind();
	bindVertexDataToShader();

	glBindVertexArray( m_vertexArray );

	for ( const auto& rectangle : m_rectangles )
	{
		m_rectangleShader->setMatrixUniform( "_TransformationMatrix", rectangle->getTransformationMatrix().transposed() );
		m_rectangleShader->setVectorUniform( "_TexCoord", Vector( rectangle->getTexCoordU(), rectangle->getTexCoordV(), 1.0f ) );

		m_rectangleShader->setFloatUniform( "_RectWidth", rectangle->getWidth() );
		m_rectangleShader->setFloatUniform( "_RectHeight", rectangle->getHeight() );
		m_rectangleShader->setFloatUniform( "_CornerRadius", rectangle->getCornerRadius() );

		m_rectangleShader->setIntegerUniform( "_FillType", static_cast<int>( rectangle->getFillType() ) );

		static const int8_t textureSlot = 0;

		switch ( rectangle->getFillType() )
		{
		case RectangleElement::FillType::Solid:
			m_rectangleShader->setColorUniform( "_FillColor", rectangle->getFillColor() );
			break;

		case RectangleElement::FillType::Linear:
			m_rectangleShader->setColorUniform( "_GradientColor1", rectangle->getGradientColor1() );
			m_rectangleShader->setColorUniform( "_GradientColor2", rectangle->getGradientColor2() );
			m_rectangleShader->setFloatUniform( "_GradientStep1", rectangle->getGradientStep1() );
			m_rectangleShader->setFloatUniform( "_GradientStep2", rectangle->getGradientStep2() );
			break;

		case RectangleElement::FillType::Image:
			glActiveTexture( GL_TEXTURE0 + textureSlot );
			glBindTexture( GL_TEXTURE_2D, rectangle->getTexture()->getTextureId() );
			m_rectangleShader->setIntegerUniform( "_FillTexture", textureSlot );
			break;

		default:
			break;
		}

		static const uint8_t vertexCount = 4;
		glDrawArrays( GL_TRIANGLE_STRIP, 0, vertexCount );
	}

	glBindVertexArray( 0 );
}

void UIRenderer::drawTextElements() const
{
	for ( const auto& text : m_texts )
	{
		m_textShader->bind();
		text->bindToShader( m_textShader );

		static const int8_t textureSlot = 0;
		glActiveTexture( GL_TEXTURE0 + textureSlot );
		glBindTexture( GL_TEXTURE_2D, text->getFont()->getTextureAtlas()->getTextureId() );
		m_textShader->setIntegerUniform( "_FontTextureAtlas", textureSlot );

		m_textShader->setColorUniform( "_Color", text->getColor() );
		m_textShader->setVectorUniform( "_Transform", text->getTransform() );
		m_textShader->setBoolUniform( "_UseShadow", text->isUseShadow() );

		m_textShader->setFloatUniform( "_SdfWidth", text->getSdfWidth() );
		m_textShader->setFloatUniform( "_SdfEdge", text->getSdfEdge() );

		uint16_t screenWidth, screenHeight;
		engine()->window()->getResolution( screenWidth, screenHeight );

		m_textShader->setVectorUniform( "_DisplaySize", Vector( screenWidth, screenHeight, 0.0f ) );

		glBindVertexArray( text->getVertexArray() );
		glDrawArrays( GL_TRIANGLES, 0, text->getNumVertices() );
	}

	glBindVertexArray( 0 );
}

