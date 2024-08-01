// Oleg Kotov

#include "fontloader.h"

std::vector<std::string> split( const std::string &s, char delim )
{
    std::vector<std::string> elems;
    split( s, delim, std::back_inserter( elems ) );
    return elems;
}

// https://www.physicsforums.com/threads/c-function-to-split-a-string-by-whitespace-ignoring-any-whitespace-in-quotes.778920/
std::vector<std::string> split_no_quotes( const std::string& str )
{
	std::string::const_iterator it( str.cbegin() ), offend( str.cend() );

	std::vector<std::string> parts;
	bool inQuotes = false;
	std::string thisWord;

	while ( it != offend )
	{
		switch ( *it )
		{
		case ' ': // whitespace
		{
			if ( !thisWord.empty() )
			{
				if ( !inQuotes )
				{
					parts.push_back( thisWord );
					thisWord.clear();
				}
				else
				{
					thisWord.push_back( *it );
				}
			}
			break;
		}
		case '"':
		{
			thisWord.push_back( *it );
			inQuotes = !inQuotes;
		}
		break;
		default: 
		{
			thisWord.push_back( *it );
		}
		break;
		}
		++it;
	}

	if ( !thisWord.empty() )
	{
		parts.push_back( thisWord );
	}

	return parts;
}

