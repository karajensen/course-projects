////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - timer.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "timer.h"
#include "glfw/glfw3.h"
#include "common.h"

namespace
{
    const float DT_MAXIMUM = 0.03f;   ///< Maximum allowed deltatime
    const float DT_MINIMUM = 0.01f;   ///< Minimum allowed deltatime
}

void Timer::UpdateTimer()
{
    const float currentTime = static_cast<float>(glfwGetTime());
    const float deltaTime = currentTime - m_previousTime;
    m_previousTime = currentTime;
    m_totalTime += deltaTime;
    m_deltaTimeCounter += deltaTime;
    m_deltaTime = Clamp(currentTime - m_previousTime, DT_MINIMUM, DT_MAXIMUM);

    if (m_deltaTimeCounter >= 1.0) //one second has passed
    {
        m_deltaTimeCounter = 0.0;
        m_fps = m_fpsCounter;
        m_fpsCounter = 0;
    }
    else
    {
        ++m_fpsCounter; 
    }
}

float Timer::GetTotalTime() const
{
    return m_totalTime;
}

float Timer::GetDeltaTime() const 
{ 
    return m_deltaTime;
}

int Timer::GetFPS() const
{
    return static_cast<int>(m_fps);
}