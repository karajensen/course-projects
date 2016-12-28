////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - particle.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"

/**
* Data for a single particle
*/
class Particle
{
public:

    /**
    * Constructor
    */
    Particle();

    /**
    * @return if this particle is currently alive
    */
    bool Alive() const;

    /**
    * @return the alpha transparency of the particle
    */
    float Alpha() const;

    /**
    * @return the id of the texture to use
    */
    int Texture() const;

    /**
    * @return the world position of the particle
    */
    const glm::vec3& Position() const;

    /**
    * @return the scale of the particle
    */
    float Size() const;

    /**
    * Ticks the particle
    * @param deltatime The time passed between ticks
    * @param direction The direction of the emitter
    * @return if the particle succesfully ticked
    */
    bool Tick(float deltatime, const glm::vec3& direction);

    /**
    * Resets the particle
    * @param lifeTime The maximum allowed life
    * @param lifeFade Time for fading in/out of life
    * @param waitTime Time for waiting before respawning
    * @param speed How fast the particle moves up
    * @param size The scale of the particle
    * @param amplitude How large the waves of the particle are
    * @param frequency How intense are the waves
    * @param texture The texture ID to render with
    * @param position The starting position of the particle
    */
    void Reset(float lifeTime, 
               float lifeFade, 
               float waitTime,
               float speed,
               float size, 
               float amplitude, 
               float frequency,
               int texture, 
               const glm::vec3& position);
    
private:

    glm::vec3 m_startPosition;    ///< The position when first given life
    glm::vec3 m_position;         ///< The position of the particle
    float m_lifeTime = 0.0f;      ///< How long has this particle been alive
    float m_waitTime = 0.0f;      ///< How long has passed while waiting
    float m_maxWaitTime = 0.0f;   ///< Time to wait before respawning
    float m_speed = 1.0f;         ///< The speed of the particle
    float m_waveSpeed = 1.0f;     ///< The wave speed of the particle
    float m_size = 1.0f;          ///< Size of the particle
    float m_alpha = 1.0f;         ///< How transparent this particle is
    float m_amplitude = 1.0f;     ///< How large the waves of the particle are
    float m_frequency = 1.0f;     ///< How intense are the waves
    float m_maxLifeTime = 0.0f;   ///< The maximum time the particle can live
    float m_lifeFade = 0.0f;      ///< When should the particle fade in/out
    int m_texture = -1;           ///< The texture to render the particle with
    bool m_alive = false;         ///< Whether this particle should be rendered
};
