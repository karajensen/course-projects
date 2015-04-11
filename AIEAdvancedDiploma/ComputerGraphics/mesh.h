////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "glm\glm.hpp"
#include "meshdata.h"

/**
* Mesh loaded from file to be rendered in the scene
*/
class Mesh : public MeshData
{
public:

    /**
    * Constructor
    * @param name The name of the mesh
    * @param shaderName The name of the shader to use
    * @param shaderID The ID of the shader to use
    * @param instances The number of default instances to create
    */
    Mesh(const std::string& name, 
         const std::string& shaderName, 
         int shaderID, 
         int instances);

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Initialises the mesh data buffer containers from file
    * @param path The full path to the mesh file
    * @param requiresUVs Whether this mesh requires UVs for texturing
    * @param requiresNormals Whether this mesh requires normals
    * @param requiresTangents Whether this mesh requires tangents/bitangents
    * @return Whether creation was successful
    */
    bool InitialiseFromFile(const std::string& path, 
                            bool requiresUVs,
                            bool requiresNormals, 
                            bool requiresTangents);

    /**
    * @return Brightness of the specular highlights
    */
    float Specularity() const;

    /**
    * @return The saturation of the bump
    */
    float Bump() const;

    /**
    * @return Ambient light multiplier
    */
    float Ambience() const;

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

private:

    /**
    * Prevent copying
    */
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    /**
    * Initialises the mesh data buffer from an OBJ file
    * @param path The full path to the mesh file
    * @param requiresUVs Whether this mesh requres UVs for texturing
    * @param requiresNormals Whether this mesh requires normals
    * @param requiresTangents Whether this mesh requires tangents/bitangents
    * @return Whether creation was successful
    */
    bool InitialiseFromOBJ(const std::string& path, 
                           bool requiresUVs,
                           bool requiresNormals, 
                           bool requiresTangents);

    /**
    * Initialises the mesh data buffer from an FBX file
    * @param path The full path to the mesh file
    * @param requiresUVs Whether this mesh requres UVs for texturing
    * @param requiresNormals Whether this mesh requires normals
    * @param requiresTangents Whether this mesh requires tangents/bitangents
    * @return Whether creation was successful
    */
    bool InitialiseFromFBX(const std::string& path,
                           bool requiresUVs,
                           bool requiresNormals, 
                           bool requiresTangents);

    float m_bump = 1.0f;                 ///< Saturation of bump
    float m_causticsAmount = 1.0f;       ///< How much of the caustics are visible
    float m_causticsScale = 1.0f;        ///< The scale of the caustic texture
    float m_specularity = 1.0f;          ///< Brightness of the specular highlights
    float m_ambience = 1.0f;             ///< Ambient light multiplier
};