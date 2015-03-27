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
    * Callbacks for rendering a mesh instance
    */
    typedef std::function<void(const glm::mat4&)> RenderInstance;

    /**
    * Holds information for a single instance of a mesh
    */
    struct Instance
    {
        glm::vec3 position;       ///< Position offset
        glm::vec3 rotation;       ///< Degress rotated around each axis
        float scale = 1.0f;       ///< Scaling of the mesh
        bool shouldRender = true; ///< Whether to render this instance
    };

    /**
    * Constructor
    * @param name The name of the mesh
    * @param shader The ID of the shader to use
    * @param instances The number of default instances to create
    */
    Mesh(const std::string& name, int shader, int instances);

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
    * Renders the mesh
    * @param renderInstance Callback to render a single mesh instance
    */
    void Render(RenderInstance renderInstance) const;

    /**
    * @return The instances of this mesh
    */
    const std::vector<Instance>& Instances() const;

    /**
    * @return The instances of this mesh
    */
    std::vector<Instance>& Instances();

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

    /**
    * Adds an instance for this mesh
    * @param position The position offset 
    * @param rotation How much to rotate 
    * @param scale The size of the mesh
    */
    void AddInstance(const glm::vec3& position,
                     const glm::vec3& rotation,
                     float scale);

    /**
    * Adds an instance at the world center with default values
    */
    void AddInstance();

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
    float m_caustics = 1.0f;             ///< How much of the caustics are visible
    float m_specularity = 1.0f;          ///< Brightness of the specular highlights
    float m_ambience = 1.0f;             ///< Ambient light multiplier
    int m_initialInstances = 0;          ///< The number of instances on load
    std::vector<Instance> m_instances;   ///< Instances of this mesh
};