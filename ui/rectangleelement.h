// Oleg Kotov

#pragma once

#include "baseapi.h"
#include <stdint.h>
#include "mathutils.h"

#include "uielement.h"
#include "vector.h"
#include "color.h"
#include "orthomatrix.h"
#include "texture.h"
#include "resourcemanager.h"

#include "okoengine.h"
#include "window.h"
#include "graphics.h"
#include "uirenderer.h"

class ENGINE_API RectangleElement : public UIElement
{
public:

	enum class FillType : uint8_t
	{
		Solid,
		Linear,
		Image
	};

public:

    RectangleElement()
        : m_pos_x( 0 )
        , m_pos_y( 0 )
        , m_width( 50 )
        , m_height( 50 )
        , m_rotation( 0.0f )
        , m_cornerRadius( 0.0f )
        , m_origin_x( 0.0f )
        , m_origin_y( 0.0f )
        , m_fillType( FillType::Solid )
        , m_fillColor{ Color( 217, 217, 217 ) }
        , m_gradientColor1{ Color( 255, 255, 255, 1.0f ) }
        , m_gradientColor2{ Color( 255, 255, 255, 0.9f ) }
        , m_gradientStep1( 0.5f )
        , m_gradientStep2( 1.0f )
        , m_texture( nullptr )
        , m_uv{ 1.0f, 1.0f }
    {
        recalculateMatrices();
        engine()->ui()->addRectangle( this );
    }

    void setPositionX( int pos )
    {
        m_pos_x = pos;
        m_matrixDirty = true;
    }

    int getPositionX() const
    {
        return m_pos_x;
    }

    void setPositionY( int pos )
    {
        m_pos_y = pos;
        m_matrixDirty = true;
    }

    int getPositionY() const
    {
        return m_pos_y;
    }

    void setWidth( int width )
    {
        m_width = width;
        m_matrixDirty = true;
    }

    int getWidth() const
    {
        return m_width;
    }

    void setHeight( int height )
    {
        m_height = height;
        m_matrixDirty = true;
    }

    int getHeight() const
    {
        return m_height;
    }

    void setRotation( float angle )
    {
        m_rotation = -angle;
        m_matrixDirty = true;
    }

    float getRotation() const
    {
        return m_rotation;
    }

    void setCornerRadius( float radius )
    {
        m_cornerRadius = radius;
    }

    float getCornerRadius() const
    {
        return m_cornerRadius;
    }

    void setOriginX( float offset )
    {
        m_origin_x = offset;
        m_matrixDirty = true;
    }

    float getOriginX() const
    {
        return m_origin_x;
    }

    void setOriginY( float offset )
    {
        m_origin_y = offset;
        m_matrixDirty = true;
    }

    float getOriginY() const
    {
        return m_origin_y;
    }

    void setFillType( FillType type )
    {
        m_fillType = type;
    }

    FillType getFillType() const
    {
        return m_fillType;
    }

    void setFillColor( const Color& color )
    {
        m_fillColor = color;
    }

    const Color& getFillColor() const
    {
        return m_fillColor;
    }

    void setGradientStep1( float value )
    {
        m_gradientStep1 = value;
    }

    float getGradientStep1() const
    {
        return m_gradientStep1;
    }

    void setGradientColor1( const Color& color )
    {
        m_gradientColor1 = color;
    }

    const Color& getGradientColor1() const
    {
        return m_gradientColor1;
    }

    void setGradientStep2( float value )
    {
        m_gradientStep2 = value;
    }

    float getGradientStep2() const
    {
        return m_gradientStep2;
    }

    void setGradientColor2( const Color& color )
    {
        m_gradientColor2 = color;
    }

    const Color& getGradientColor2() const
    {
        return m_gradientColor2;
    }

    void setTexture( Texture* texture, bool setToTextureSize = false )
    {
        m_texture = texture;

        if ( setToTextureSize && texture )
        {
            m_width = texture->getWidth();
            m_height = texture->getHeight();
            m_matrixDirty = true;
        }
    }

    void setTexture( const std::string& filename, bool setToTextureSize = false )
    {
        Texture* texture = engine()->resources()->getTexture( filename );
        setTexture( texture, setToTextureSize );
    }

    Texture* getTexture() const
    {
        return m_texture;
    }

    void setTexCoordU( float value )
    {
        m_uv[0] = value;
    }

    float getTexCoordU() const
    {
        return m_uv[0];
    }

    void setTexCoordV( float value )
    {
        m_uv[1] = value;
    }

    float getTexCoordV() const
    {
        return m_uv[1];
    }

    const Matrix& getTransformationMatrix()
    {
        if ( m_matrixDirty )
        {
            m_matrixDirty = false;
            recalculateMatrices();
        }

        return m_transformationMatrix;
    }

    void setMaxCornerRadius()
    {
        m_cornerRadius = std::min( m_width, m_height ) * 0.5f;
    }

    float getMaxCornerRadius() const
    {
        return std::min( m_width, m_height ) * 0.5f;
    }

    void centerOnScreen()
    {
        uint16_t screenWidth, screenHeight;
	    engine()->window()->getResolution( screenWidth, screenHeight );

		int offsetX = m_width * m_origin_x;
		int offsetY = m_height * m_origin_y;

		m_pos_x = ( screenWidth - m_width ) * 0.5f + offsetX;
		m_pos_y = ( screenHeight - m_height ) * 0.5f + offsetY;

        m_matrixDirty;
    }

private:

    Matrix transformation4x4( float x, float y, float angle, float sx, float sy, float ox, float oy, float kx, float ky )
    {
        Matrix m;

        float c = cosf( angle );
        float s = sinf( angle );

        const float sxc = sx * c;
        const float syc = sy * c;
        const float sxs = sx * s;
        const float sys = sy * s;

        const float tx = -ox * sxc - oy * sys + x;
        const float ty = ox * sxs - oy * syc + y;

        m[0][0] = sxc;
        m[0][1] = sys;
        m[0][3] = tx;

        m[1][0] = -sxs;
        m[1][1] = syc;
        m[1][3] = ty;

        m[2][2] = 1.0f;
        m[3][3] = 1.0f;

        return m;
    }

    void recalculateMatrices()
    {
		int32_t win_width = engine()->graphics()->getWindowWidth();
		int32_t win_height = engine()->graphics()->getWindowHeight();

        Matrix transformationMatrix = transformation4x4( m_pos_x, m_pos_y, m_rotation * Deg2Rad, m_width, m_height, m_origin_x, m_origin_y, 0.0f, 0.0f );
        Matrix projectionMatrix = OrthoMatrix( 0.0f, win_width, win_height, 0.0f, -1.0f, 1.0f );

        m_transformationMatrix = projectionMatrix * transformationMatrix;
    }

private:

    //
    Matrix m_transformationMatrix;

    // The position of the rectangle on the screen along the X-axis in pixels
    int m_pos_x;

    // The position of the rectangle on the screen along the Y-axis in pixels
    int m_pos_y;

    // The width of the rectangle in pixels
    int m_width;

    // The height of the rectangle in pixels
    int m_height;

    // The rotation angle of the rectangle in degrees
    float m_rotation;

    // The radius for rounding the corners of the rectangle
    float m_cornerRadius;

    // The offset of the origin point along the X-axis relative to the rectangle's position
    float m_origin_x;

    // The offset of the origin point along the Y-axis relative to the rectangle's position
    float m_origin_y;

    // The fill type of the rectangle, whether it's a solid color, gradient, or texture
    FillType m_fillType;

    // An array containing the RGBA color components of the fill color for the rectangle
    Color m_fillColor;

    // The threshold value for transitioning between gradient colors
    float m_gradientStep1;

    // An array containing the RGBA color components of the first gradient color
    Color m_gradientColor1;

    // The threshold value for transitioning between gradient colors
    float m_gradientStep2;

    // An array containing the RGBA color components of the second gradient color
    Color m_gradientColor2;

    // The texture descriptor used for filling the rectangle
    class Texture* m_texture;

    // An array containing the texture coordinates (UV coordinates) for mapping the texture onto the rectangle
    float m_uv[2]; // fillTextureTexCoord

    // Transformation matrix dirty flag
    bool m_matrixDirty = true;
};

