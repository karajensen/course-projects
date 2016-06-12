////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - timer.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include "timer.h"

Timer::Timer() :
    m_frequency(0.0),
    m_previousTime(0.0),
    m_deltaTime(0.0),
    m_deltaTimeCounter(0.0),
    m_fps(0),
    m_fpsCounter(0)
{
}

void Timer::StartTimer()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&m_timer);

    m_frequency = static_cast<double>(frequency.QuadPart);
    m_previousTime = static_cast<double>(m_timer.QuadPart);
}

void Timer::UpdateTimer()
{
    QueryPerformanceCounter(&m_timer);
    double currentTime = static_cast<double>(m_timer.QuadPart);

    m_deltaTime = (currentTime - m_previousTime) / m_frequency;
    m_deltaTimeCounter += m_deltaTime;
    if (m_deltaTimeCounter >= 1.0) //one second has passed
    {
        m_deltaTimeCounter = 0.0;
        m_fps = m_fpsCounter;
        m_fpsCounter = 0;
    }
    
    ++m_fpsCounter; //increment frame counter
    m_previousTime = currentTime;
}

double Timer::GetDeltaTime() const 
{ 
    return m_deltaTime; 
}

unsigned int Timer::GetFPS() const
{
    return m_fps;
}