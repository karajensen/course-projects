////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "grid.h"

/**
* Procedurally generated mesh
*/
class Terrain : public Grid
{
public:

    /**
    * Constructor
    * @param name The name of the terrain
    * @param shaderID The id of the shader to use
    * @param pixels The pixels of the height map
    */
    Terrain(const std::string& name, int shaderID,
        const std::vector<unsigned int>& pixels);

    /**
    * Initialises the terrain
    * @param position The center of the grid
    * @param uvStretch Texture stretch multiplier
    * @param minHeight The minimum height of the terrain
    * @param maxHeight The maximum height of the terrain
    * @param spacing The spacing between vertices
    * @param size How many rows/columns for the grid
    * @param hasNormals Whether the terrain requires normals
    * @param hasNormals Whether the terrain requires tangents
    * @return whether call was successful
    */
    bool Initialise(const glm::vec3& position,
                    const glm::vec2& uvStretch,
                    float minHeight,
                    float maxHeight,
                    float spacing, 
                    int size,
                    bool hasNormals,
                    bool hasTangents);

    /**
    * Reloads the terrain
    */
    void Reload();

    /**
    * @return Brightness of the specular highlights
    */
    float Specularity() const;

    /**
    * @return Ambient light multiplier
    */
    float Ambience() const;

    /**
    * @return The saturation of the bump
    */
    float Bump() const;

    /**
    * @return Caustics multiplier
    */
    float Caustics() const;

    /**
    * Sets the Brightness of the specular highlights
    */
    void Specularity(float value);

    /**
    * Sets The saturation of the bump
    */
    void Bump(float value);

    /**
    * Sets the Ambient light multiplier
    */
    void Ambience(float value);

    /**
    * Sets the Caustics multiplier
    */
    void Caustics(float value);

private:

    /**
    * Prevent copying
    */
    Terrain(const Terrain&) = delete;
    Terrain& operator=(const Terrain&) = delete;

    /**
    * Generates terrain using the given type
    */
    void GenerateTerrain();

    float m_maxHeight = 1.0f;   ///< The maximum height of the terrain
    float m_minHeight = 0.0f;   ///< The minimum height of the terrain
    float m_bump = 1.0f;        ///< Saturation of bump
    float m_caustics = 1.0f;    ///< How much of the caustics are visible
    float m_specularity = 1.0f; ///< Brightness of the specular highlights
    float m_ambience = 1.0f;    ///< Ambient light multiplier

    const std::vector<unsigned int>& m_pixels; ///< The pixel of the height map
};                                