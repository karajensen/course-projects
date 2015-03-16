////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - animation.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>

/**
* Manages cycling through multiple textures
*/
class Animation
{
public:

    /**
    * Index for avaliable animations
    */
    enum Index
    {
        ID_CAUSTICS,
        MAX_ANIMATION
    };

    /**
    * Constructor
    */
    Animation() = default;
            
    /**
    * Adds a texture frame 
    * @param ID the unique ID of the texture to add
    */
    void AddFrame(int ID);

    /**
    * @return the current frame this texture is in
    */
    int GetFrame() const;

    /**
    * Ticks the animated texture
    */
    void Tick(float deltatime);

private:

    /**
    * Prevent copying
    */
    Animation(const Animation&) = delete;
    Animation& operator=(const Animation&) = delete;

    std::vector<int> m_frames;        ///< All frames to animate
    int m_selectedFrame = 0;          ///< The currently selected frame
    float m_timePassed = 0.0f;        ///< The time passed since last frame switch

};