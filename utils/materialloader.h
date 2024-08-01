// Oleg Kotov

#pragma once

#include "okobase.h"
#include "vector.h"
#include "color.h"

class Material;

class ENGINE_API MaterialLoader
{    
public:
    
    Material* load( const std::string& filepath );

private:

    // TODO: move to utils

    Vector stringToVector( const char* str );
    Color stringToColor( const char* str );
};

// assetType / loader - playcanvasStyle?

