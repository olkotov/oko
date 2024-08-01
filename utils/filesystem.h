// Oleg Kotov

#pragma once

#include "okobase.h"
#include <fstream>

template <typename UserType>
void read( std::ifstream& stream, UserType* instance, size_t size = sizeof( UserType ) )
{
	stream.read( (char*)instance, size );
}

class ENGINE_API Filesystem
{
public:

	std::string getWorkingDir() const;

	// setAssetsDir( const std::string& dir );
	const std::string& getAssetsDir() const { return m_assetsPath; }

	std::string readFileData( const std::string& file ) const;
	bool readFileDataBin( const std::string& file, std::vector<char>& outBuffer ) const;

private:

	std::string m_assetsPath;
};

extern ENGINE_API class Filesystem Filesystem;

