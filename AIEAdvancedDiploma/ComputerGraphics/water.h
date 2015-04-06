////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "grid.h"
#include <array>

/**
* Data for a water-shaded mesh
*/
class Water : public Grid
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
    * @param height The height of the water
    * @param spacing The spacing between vertices
    * @param size How many rows/columns for the grid
    */
    bool Initialise(float height, float spacing, int size);

    /**
    * Sets the position of the water
    * @param position The world position of the center
    * @param flippedX Whether to flip the water in the x axis for tiling
    * @param flippedZ Whether to flip the water in the z axis for tiling
    */
    void Set(const glm::vec2& position, bool flippedX, bool flippedZ);

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

    /**
    * Adds a new instance for the water
    * @param position The world position of the center
    * @param flippedX Whether to flip the water in the x axis for tiling
    * @param flippedZ Whether to flip the water in the z axis for tiling
    */
    void AddInstance(const glm::vec2& position, bool flippedX, bool flippedZ);

    /**
    * Sets the instance of the water
    * @param index The index of the instance to set
    * @param position The world position of the center
    * @param flippedX Whether to flip the water in the x axis for tiling
    * @param flippedZ Whether to flip the water in the z axis for tiling
    */
    void SetInstance(int index, const glm::vec2& position, bool flippedX, bool flippedZ);

private:

    /**
    * Prevent copying
    */
    Water(const Water&) = delete;
    Water& operator=(const Water&) = delete;

    float m_height = 0.0f;               ///< The initial height of the terrain
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