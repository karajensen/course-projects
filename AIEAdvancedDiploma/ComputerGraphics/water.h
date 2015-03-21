////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "meshdata.h"
#include <array>

/**
* Data for a water-shaded mesh
*/
class Water : public MeshData
{
public:

    /**
    * Data for a water wave
    */
    struct Wave
    {
        float frequency = 0.0f;   ///< Frequency of the wave
        float amplitude = 0.0f;   ///< Amplitude of the wave
        float phase = 0.0f;       ///< Phase of the wave
        float directionX = 0.0f;  ///< Direction the wave moves
        float directionZ = 0.0f;  ///< Direction the wave moves
        static const int MAX = 2; ///< Maximum allowed waves
    };

    /**
    * Constructor
    * @param name The name of the water
    * @param shaderID The ID of the shader to use
    */
    Water(const std::string& name, int shaderID);

    /**
    * Initialises the water
    * @param position The center of the grid
    * @param spacing The spacing between vertices
    * @param rows How many rows for the grid
    * @param columns How many columns for the grid
    */
    bool Initialise(const glm::vec3& position, float spacing, int rows, int columns);

    /**
    * @return The Wave information for the water
    */
    const std::vector<Wave>& Waves() const;

    /**
    * @return The Velocity of the bump movement
    */
    const glm::vec2& BumpVelocity() const;

    /**
    * @return The overall speed of the wave
    */
    float Speed() const;

    /**
    * @return The scale of the water texture
    */
    const glm::vec2& UVScale() const;

    /**
    * @return The fresnal scale, bias and power 
    */
    const glm::vec3& Fresnal() const;

    /**
    * @return Colour intensity of the shallow water
    */
    const glm::vec4& Shallow() const;

    /**
    * @return Colour intensity of the deep water
    */
    const glm::vec4& Deep() const;

    /**
    * @return Colour of the reflections
    */
    const glm::vec3& ReflectionTint() const;

    /**
    * @return the intensity of the reflections
    */
    float ReflectionIntensity() const;

    /**
    * @return The saturation of the bump
    */
    float Bump() const;

private:

    /**
    * Prevent copying
    */
    Water(const Water&) = delete;
    Water& operator=(const Water&) = delete;

    float m_speed = 0.0f;                ///< Overall speed of the wave
    float m_bump = 0.0f;                 ///< Saturation of bump
    glm::vec2 m_bumpVelocity;            ///< Velocity of the bump movement
    glm::vec2 m_uvScale;                 ///< Scale of the water texture
    glm::vec3 m_fresnal;                 ///< The fresnal scale, bias and power
    glm::vec4 m_shallowColour;           ///< Colour intensity of the shallow water
    glm::vec4 m_deepColour;              ///< Colour intensity of the deep water
    glm::vec3 m_reflectionTint;          ///< Colour of the reflections
    float m_reflection = 0.0f;           ///< Intensity of the reflections
    std::vector<Water::Wave> m_waves;    ///< Wave infomation
};