// Oleg Kotov

//#include "clock.h"
//
//Clock::Clock()
//{
//	m_startTime = std::chrono::high_resolution_clock::now();
//}
//
//float Clock::getTimeSinceStartup() const
//{
//    auto currentTime = std::chrono::high_resolution_clock::now();
//    return std::chrono::duration_cast<std::chrono::duration<float>>( currentTime - m_startTime ).count();
//}
//
//float Clock::update()
//{
//    float currentTime = getTimeSinceStartup();
//    m_deltaTime = currentTime - m_previousTime;
//    m_previousTime = currentTime;
//    
//    return m_deltaTime * m_timeScale;
//}
//
//float Clock::getDeltaTime() const
//{
//    return m_deltaTime * m_timeScale;
//}
//
//
//float Clock::getUnscaledDeltaTime() const
//{
//    return m_deltaTime;
//}
//
//
//void Clock::setTimeScale( float timeScale )
//{
//    m_timeScale = timeScale;
//}
//
//
//float Clock::getTimeScale() const
//{
//    return m_timeScale;
//}
//
//uint64_t Clock::getUnixTime() const
//{
//    return static_cast<uint64_t>( std::time( nullptr ) );
//}

