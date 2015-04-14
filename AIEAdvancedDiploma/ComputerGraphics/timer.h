////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - timer.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>

/**
* FPS class for determining frame rate and delta time
*/
class Timer
{
public:

    /**
    * Stops/Starts the timer to determine delta-time and fps
    */
    void UpdateTimer();

    /**
    * @return The time passed since last frame in seconds
    */
    float GetDeltaTime() const;

    /**
    * @return the total time passed since starting the simulation
    */
    float GetTotalTime() const;

    /**
    * @return the frames per second
    */
    int GetFPS() const;

private:

    float m_previousTime = 0.0;       ///< The previous time queried
    float m_deltaTime = 0.0;          ///< The time passed since last frame in seconds
    float m_deltaTimeCounter = 0.0;   ///< Combined timestep between frames up to 1 second
    float m_totalTime = 0.0;          ///< The time passed since start of the timer
    unsigned int m_fps = 0;           ///< Amount of frames rendered in 1 second
    unsigned int m_fpsCounter = 0;    ///< Amount of frames rendered since delta time counter began
};

