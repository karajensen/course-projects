////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - animation.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "animation.h"
              
void Animation::AddFrame(int ID)
{
    m_frames.push_back(ID);
}

int Animation::GetFrame() const
{
    return m_frames[m_selectedFrame];
}

void Animation::Tick(float deltatime)
{
    m_timePassed += deltatime;

    const float secondsUntilSwitch = 0.065f;
    if (m_timePassed >= secondsUntilSwitch)
    {
        const int maxFrame = static_cast<int>(m_frames.size())-1;
        m_selectedFrame = m_selectedFrame == maxFrame ? 0 : m_selectedFrame + 1;
        m_timePassed = 0.0f;
    }
}