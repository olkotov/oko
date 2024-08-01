// Oleg Kotov

#pragma once

#include "okobase.h"

#include <vector>
#include <string>
#include <chrono>

struct ENGINE_API ProfileSample
{
	ProfileSample( const std::string& name, float value, const std::string& units )
	{
		this->name = name;
		this->value = value;
		this->units = units;
	}

	std::string name;
	float value;
	std::string units;
};

class ENGINE_API Profiler
{
public:

	Profiler();
	~Profiler();

	void addStats( const std::string& name, float value, const std::string& units = " ms" );
	float getStatsValue( const std::string& name ) const;

	const std::vector<ProfileSample>& getStats() const;

	void clear();
	void drawOnScreen();

private:

	class ProfilerRenderer* m_profileRenderer = nullptr;
	std::vector<ProfileSample> m_stats;
};

struct ENGINE_API AutoProfile
{
public:

	AutoProfile( const std::string& name, const std::string& units = " ms" );
	~AutoProfile();

private:

	std::string m_name;
	std::string m_units;
	std::chrono::steady_clock::time_point m_startTime;
};

#define PROFILE( name, units ) AutoProfile p( name, units )

