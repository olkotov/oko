// Oleg Kotov

#pragma once

#include "okobase.h"
#include <functional>

using ConsoleCommandFunction = std::function<void()>;

struct ConsoleVariable
{
public:

	ConsoleVariable( const std::string& name, const std::string& defaultValue, const std::string& description )
	: m_name( name )
	, m_defaultValue( defaultValue )
	, m_description( description )
	{
		// register to console

		setValue( m_defaultValue );
	}

	void setValue( const std::string& value )
	{
		internalSetValue( value.c_str() );
	}

	void setValue( int value )
	{
		internalSetIntValue( value );
	}

	void setValue( float value )
	{
		internalSetFloatValue( value );
	}

	const std::string& getName() const
	{
		return m_name;
	}

	const std::string& getDescription() const
	{
		return m_description;
	}

	float getFloat() const { return m_floatValue; }
	int getInt() const { return m_intValue; }
	bool getBool() const { return !!getInt(); }
	const std::string& getString() const { return m_strValue; }

private:

	std::string m_name; // displayName // ball_jump_height
	std::string m_description; // tooltip // help // Jump height of the ball.
	std::string m_defaultValue; // 2.0
	std::string m_value;

	void internalSetValue( const char* value )
	{
		m_strValue = value;
		m_floatValue = (float)atof( value );
		m_intValue = (int)m_floatValue;
	}

	void internalSetFloatValue( float value )
	{
		if ( value == m_floatValue ) return;

		char tempVal[32];
		snprintf( tempVal, sizeof( tempVal ), "%f", m_floatValue );

		m_strValue = tempVal;
		m_floatValue = value;
		m_intValue = (int)value;
	}

	void internalSetIntValue( int value )
	{
		char tempVal[32];
		snprintf( tempVal, sizeof( tempVal ), "%d", m_intValue );
		
		m_strValue = tempVal;
		m_floatValue = (float)value;
		m_intValue = value;
	}

	std::string m_strValue;
	float m_floatValue;
	int m_intValue;
};

// typedef void(*ccommand_func)();

struct ConsoleCommand
{
	ConsoleCommand( const std::string& name, const std::string& description, ConsoleCommandFunction command )
	: m_name( name )
	, m_description( description )
	, m_command( command )
	{
		// register to console
	}

	std::string m_name; // level level_01
	std::string m_description; // Jump height of the ball.
	// ccommand_func m_command;
	ConsoleCommandFunction m_command;

	// exec( const std::vector<std::string>& args )
};

class ENGINE_API Console
{
public:

	void registerVariable( const ConsoleVariable& cvar )
	{
		// add cvar to map
	}
	
	void registerCommand( const ConsoleVariable& ccommand )
	{
		// add ccommand to map
	}

	void run( const std::string& command ) const
	{
		// find ccommand and call it
	}

	ConsoleVariable findVariable( const std::string& name )
	{
		// find cvar and get it
	}

private:

	std::map<std::string, ConsoleVariable> m_vars;
	std::map<std::string, ConsoleCommand> m_commans;
};

/*

	// message()
	// warning()

	Unigine.Console.Run( "world_reload" );

	engine()->console()->run( "map playtest" );
	// exec

	ConVar player_health;

	engine()->console()->registerConsoleVariable( "ball_jump_height" );
	engine()->console()->registerConsoleCommand( "ball_jump_height" );

	static const auto cvar = engine()->console()->findConsoleVariable( "ball_jump_height" );

	ConVar ball_jump_height( "ball_jump_height", "Jump height of the ball.", "1.0" );

	float value = ball_jump_height->getFloat();

	ConVar	obj_sentrygun_range_max( "obj_sentrygun_range_max","1600", FCVAR_NONE, "Sentrygun's max targeting range." );

	https://github.com/olkotov/tf2/blob/master/tier1/convar.cpp#L645

*/

