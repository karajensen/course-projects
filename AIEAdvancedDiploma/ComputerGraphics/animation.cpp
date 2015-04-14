////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - animation.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "animation.h"
#include "tweaker.h"
    
void Animation::AddFrame(int ID)
{
    m_frames.push_back(ID);
}

int Animation::GetFrame() const
{
    return m_frames[m_selectedFrame];
}

void Animation::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddEntry("Frames", [this](){ return std::to_string(m_frames.size()); });
    tweaker.AddEntry("Speed", &m_speed, TW_TYPE_FLOAT, 0.001f);
}

void Animation::Tick(float deltatime)
{
    m_timePassed += deltatime;
    if (m_timePassed >= m_speed)
    {
        const int maxFrame = static_cast<int>(m_frames.size())-1;
        m_selectedFrame = m_selectedFrame == maxFrame ? 0 : m_selectedFrame + 1;
        m_timePassed = 0.0f;
    }
}