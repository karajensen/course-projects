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
    * The type of terrain to generate
    */
    enum Type
    {
        DIAMOND_SQUARE
    };

    /**
    * Constructor
    * @param name The name of the terrain
    * @param shaderID The id of the shader to use
    */
    Terrain(const std::string& name, int shaderID);

    /**
    * Initialises the terrain
    * @param type The type of terrain algorithm to use
    * @param position The center of the grid
    * @param spacing The spacing between vertices
    * @param size How many rows/columns for the grid
    * @return whether call was successful
    */
    bool Initialise(Type type, 
                    const glm::vec3& position, 
                    float spacing, 
                    int size);

    /**
    * Reloads the terrain
    * @return whether call was successful
    */
    bool Reload();

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

    /**
    * Generates terrain using the seamless diamond square algorithm
    */
    void GenerateDiamondSquareTerrain();

    Type m_type;                   ///< The type of terrain to generate
    float m_bump = 1.0f;           ///< Saturation of bump
    float m_caustics = 1.0f;       ///< How much of the caustics are visible
    float m_specularity = 1.0f;    ///< Brightness of the specular highlights
    float m_ambience = 1.0f;       ///< Ambient light multiplier
};                                