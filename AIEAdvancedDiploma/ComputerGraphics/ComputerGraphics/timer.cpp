////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - timer.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "timer.h"
#include "glfw/glfw3.h"
#include "common.h"
#include "tweaker.h"

Timer::Timer()
{
    m_sectionStart.assign(0.0f);
    m_sectionTime.assign(0.0f);
}

void Timer::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddEntry("Frames Per Second", &m_fps, TW_TYPE_INT32, true);

    const int precision = 8;
    tweaker.AddFltEntry("Total Time", &m_deltaTime, precision);
    tweaker.AddFltEntry("Total Rendering", &m_sectionTime[RENDERING], precision);
    tweaker.AddFltEntry("Render Scene", &m_sectionTime[RENDER_SCENE], precision);
    tweaker.AddFltEntry("Render Meshes", &m_sectionTime[RENDER_WATER], precision);
    tweaker.AddFltEntry("Render Water", &m_sectionTime[RENDER_WATER], precision);
    tweaker.AddFltEntry("Render Terrain", &m_sectionTime[RENDER_TERRAIN], precision);
    tweaker.AddFltEntry("Render Emitters", &m_sectionTime[RENDER_EMITTERS], precision);
    tweaker.AddFltEntry("Render Emitters", &m_sectionTime[RENDER_SHADOWS], precision);
    tweaker.AddFltEntry("Render Effects", &m_sectionTime[RENDER_EFFECTS], precision);
    tweaker.AddFltEntry("Render Blur", &m_sectionTime[RENDER_BLUR], precision);
    tweaker.AddFltEntry("Render Post", &m_sectionTime[RENDER_POST], precision);
    tweaker.AddFltEntry("Render GUI", &m_sectionTime[RENDER_GUI], precision);
    tweaker.AddFltEntry("Update Scene", &m_sectionTime[SCENE_UPDATE], precision);
}

void Timer::UpdateTimer()
{
    const float currentTime = static_cast<float>(glfwGetTime());
    m_deltaTime = currentTime - m_previousTime;
    m_previousTime = currentTime;
    m_totalTime += m_deltaTime;
    m_deltaTimeCounter += m_deltaTime;

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

void Timer::StartSection(TimedSection section)
{
    m_sectionStart[section] = static_cast<float>(glfwGetTime());
}

void Timer::StopSection(TimedSection section)
{
    m_sectionTime[section] = static_cast<float>(glfwGetTime()) - m_sectionStart[section];
}

float Timer::GetTotalTime() const
{
    return m_totalTime;
}

float Timer::GetDeltaTime() const 
{ 
    return m_deltaTime;
}