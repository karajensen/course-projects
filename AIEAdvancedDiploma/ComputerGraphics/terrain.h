////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "grid.h"
#include "meshAttributes.h" 

/**
* Procedurally generated mesh
*/
class Terrain : public Grid, public MeshAttributes
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
    bool Initialise(float uvStretch,
                    float minHeight,
                    float maxHeight,
                    float height,
                    float spacing, 
                    int size,
                    bool hasNormals,
                    bool hasTangents);

    /**
    * Reloads the terrain
    */
    void Reload();

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

    float m_height = 0.0f;     ///< The initial height of the terrain
    float m_maxHeight = 1.0f;  ///< The maximum height offset of the terrain
    float m_minHeight = 0.0f;  ///< The minimum height offset of the terrain
    const std::vector<unsigned int>& m_pixels; ///< The pixel of the height map
};                                