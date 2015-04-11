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
    * @param shaderName The name of the shader to use
    * @param pixels The pixels of the height map
    */
    Terrain(const std::string& name,
            const std::string& shaderName,    
            int shaderID,
            const std::vector<unsigned int>& pixels);

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Initialises the terrain
    * @param uvStretch Texture stretch multiplier
    * @param minHeight The minimum height offset of the terrain
    * @param maxHeight The maximum height offset of the terrain
    * @param height The starting height of the terrain
    * @param spacing The spacing between vertices
    * @param size How many rows/columns for the grid
    * @param hasNormals Whether the terrain requires normals
    * @param hasNormals Whether the terrain requires tangents
    * @return whether call was successful
    */
    bool Initialise(const glm::vec2& uvStretch,
                    float minHeight,
                    float maxHeight,
                    float height,
                    float spacing, 
                    int size,
                    bool hasNormals,
                    bool hasTangents);

    /**
    * Sets the position of the terrain
    * @param position The position of the terrain
    */
    void Set(const glm::vec2& position);

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
    float CausticsAmount() const;

    /**
    * @return Caustics scale
    */
    float CausticsScale() const;

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
    void CausticsAmount(float value);

    /**
    * Sets the Caustics scale
    */
    void CausticsScale(float value);

    /**
    * Adds a new instance of the terrain
    * @param position The position of the instance
    */
    void AddInstance(const glm::vec2& position);

    /**
    * Sets the instance of the terrain
    * @param index The index of the instance to set
    * @param position The position of the instance
    */
    void SetInstance(int index, const glm::vec2& position);

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

    float m_height = 0.0f;          ///< The initial height of the terrain
    float m_maxHeight = 1.0f;       ///< The maximum height offset of the terrain
    float m_minHeight = 0.0f;       ///< The minimum height offset of the terrain
    float m_bump = 1.0f;            ///< Saturation of bump
    float m_causticsAmount = 1.0f;  ///< How much of the caustics are visible
    float m_causticsScale = 1.0f;   ///< The scale of the caustic texture
    float m_specularity = 1.0f;     ///< Brightness of the specular highlights
    float m_ambience = 1.0f;        ///< Ambient light multiplier

    const std::vector<unsigned int>& m_pixels; ///< The pixel of the height map
};                                