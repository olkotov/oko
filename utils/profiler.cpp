// Oleg Kotov

#include "profiler.h"
#include "okoengine.h"
#include "profilerrenderer.h"

Profiler::Profiler()
{
	m_profileRenderer = new ProfilerRenderer();
}

Profiler::~Profiler()
{
	SafeDelete( m_profileRenderer );
}

void Profiler::addStats( const std::string& name, float value, const std::string& units /* = " ms" */ )
{
	for ( auto& sample : m_stats )
	{
		if ( sample.name == name )
		{
			sample.value = value;
			return;
		}
	}

	m_stats.emplace_back( name, value, units );
}

float Profiler::getStatsValue( const std::string& name ) const
{
	for ( const auto& sample : m_stats )
	{
		if ( sample.name == name ) return sample.value;
	}

	return -1.0f;
}

const std::vector<ProfileSample>& Profiler::getStats() const
{
	return m_stats;
}

void Profiler::clear()
{
	m_profileRenderer->clear();
}

void Profiler::drawOnScreen()
{
	m_profileRenderer->draw( this );
}

AutoProfile::AutoProfile( const std::string& name, const std::string& units /* = " ms" */ )
{
	m_name = name;
	m_units = units;
	m_startTime = std::chrono::high_resolution_clock::now();
}

AutoProfile::~AutoProfile()
{
	auto endTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>( endTime - m_startTime ).count();

	engine()->profiler()->addStats( m_name, elapsedTime * 1000, m_units );
}

