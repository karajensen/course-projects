////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include "meshdata.h"

/**
* Procedurally generated mesh
*/
class Terrain : public MeshData
{
public:

    /**
    * Constructor
    * @param name The name of the terrain
    * @param shaderID The id of the shader to use
    */
    Terrain(const std::string& name, int shaderID); 

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
    * @return Whether back facing polygons are culled
    */
    bool BackfaceCull() const;

private:

    float m_bump = 1.0f;         ///< Saturation of bump
    float m_caustics = 1.0f;     ///< How much of the caustics are visible
    float m_specularity = 1.0f;  ///< Brightness of the specular highlights
    float m_ambience = 1.0f;     ///< Ambient light multiplier
};