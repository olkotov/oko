// Oleg Kotov

#include "filesystem.h"
#include "log.h"

#include <SDL2/SDL.h>

class Filesystem Filesystem;

std::string Filesystem::getWorkingDir() const
{
	return std::string( SDL_GetBasePath() );
}

std::string Filesystem::readFileData( const std::string& filename ) const
{
    std::ifstream file( filename.c_str() );
    
    if ( file.is_open() == false )
    {
		Warning( "Unable to open file '%s'! Skipping...\n", filename.c_str() );
        return "";
    }
    
	std::istreambuf_iterator<char> beginIter( file ), endIter;
	std::string content( beginIter, endIter );

	file.close();

    return content;
}

bool Filesystem::readFileDataBin( const std::string& path, std::vector<char>& outBuffer ) const
{
	std::ifstream file( path, std::ios::binary );

	if ( file.is_open() == false )
	{
		Warning( "failed to open file '%s'", path.c_str() );
		return false;
	}

	std::istreambuf_iterator<char> begin_iter( file ), end_iter;
	std::vector<char> data( begin_iter, end_iter );

	outBuffer = std::move( data );

	file.close();

	return true;
}

