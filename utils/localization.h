// Oleg Kotov

#pragma once

#include "okobase.h"

enum Locale
{
	Locale_En,
	Locale_Ru
};

class ENGINE_API Localization
{
public:

	// initFromFile( filename );
	// setLocale( Locale_Ru )
    
    // wchar_t getLocalizedString( const char* id );
	// std::string getText( const std::string& key ) const;

private:

	std::map<std::string, std::string> m_keyval;
    Locale m_locale = Locale_En;
};

/*

	https://developer.android.com/guide/topics/resources/string-resource#Plurals

    // quantity=["zero" | "one" | "two" | "few" | "many" | "other"]

    // ru
    function getQuantityString( quantity, $one, $two, $five )
    {
        $n = abs( quantity );
        $n %= 100;

        if ( $n >= 5 && $n <= 20 )
        {
            return $five;
        }

        $n %= 10;

        if ( $n == 1 )
        {
            return $one;
        }

        if ( $n >= 2 && $n <= 4 )
        {
            return $two;
        }

        return $five;
    }

    int32 numDragons = 5;
    std::string text = getQuantityString( numDragons, "дракон", "дракона", "драконов" );

*/

