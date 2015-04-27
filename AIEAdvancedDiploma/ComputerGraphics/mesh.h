////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "meshdata.h"
#include "meshAttributes.h"

/**
* Mesh loaded from file to be rendered in the scene
*/
class Mesh : public MeshData, public MeshAttributes
{
public:

    /**
    * Constructor
    * @param name The name of the mesh
    * @param shaderName The name of the shader to use
    * @param shaderID The ID of the shader to use
    */
    Mesh(const std::string& name, 
         const std::string& shaderName, 
         int shaderID);

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
};